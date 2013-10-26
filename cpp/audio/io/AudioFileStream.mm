//
//  AudioFileStream.mm
//  lantern
//

#import <AudioToolbox/AudioToolbox.h>
#import "CAStreamBasicDescription.h"

#include "AudioFileStream.h"
#include <stdio.h>

// construct / destroy / reset

AudioFileStream::AudioFileStream() {
    buffer = NULL;
    doubleBuffer = NULL;
    xafRef_ptr = NULL;
    fileIdx = 0;
    
    clear();
}

AudioFileStream::~AudioFileStream() {
    clear();
}

void AudioFileStream::clear() {
    if (buffer) {
		free(buffer);
        buffer = NULL;
    }
    if (doubleBuffer) {
        free(doubleBuffer);
        doubleBuffer = NULL;
    }
	
	if (xafRef_ptr) {
        ExtAudioFileDispose((ExtAudioFileRef)xafRef_ptr);
        xafRef_ptr = NULL;
    }
    
    length = loadedLengthCurrentBuffer = loadedLengthDoubleBuffer = numChannels = 0;
    isLoaded = false;
    useDoubleBuffering = false;
	rateRatio = 0;
	numHeaderFrames = 0;
    
    reset();
}

void AudioFileStream::reset() {
    seek(0);
}


// public getters / setters

void AudioFileStream::setEnableDoubleBuffering(bool enable) {
    useDoubleBuffering = enable;
    if (!enable && doubleBuffer) {
        free(doubleBuffer);
        doubleBuffer = NULL;
        loadedLengthDoubleBuffer = 0;
    }
}

bool AudioFileStream::getIsLoaded() {
    return isLoaded;
}

unsigned int AudioFileStream::getLength() {
    return length;
}

unsigned int AudioFileStream::getLoadedLength() {
    return loadedLengthCurrentBuffer;
}

unsigned int AudioFileStream::getNumChannels() {
    return numChannels;
}


// big fucking load method to establish and configure our file handle

bool AudioFileStream::open(const char* filepath) {
    if (isLoaded)
        clear();
    
    ExtAudioFileRef xafRef = (ExtAudioFileRef)xafRef_ptr;
    NSURL* assetURL = [NSURL URLWithString:[NSString stringWithUTF8String:filepath]];
	
	@try {
		OSStatus error = noErr;
		
		// open the file
		error = ExtAudioFileOpenURL((__bridge CFURLRef)assetURL, &xafRef);
		if (error) {
			fprintf(stdout, "AudioFileStream failed to load %s at ExtAudioFileOpenURL\n", filepath);
            return false;
        }
		
		// allocate main data buffer
		UInt32 propSize = sizeof(fileLengthFrames);
		ExtAudioFileGetProperty(xafRef, kExtAudioFileProperty_FileLengthFrames, &propSize, &fileLengthFrames);
		
		CAStreamBasicDescription clientFormat;
		propSize = sizeof(clientFormat);
		ExtAudioFileGetProperty(xafRef, kExtAudioFileProperty_FileDataFormat, &propSize, &clientFormat);
		
		rateRatio = LANTERN_AUDIO_SAMPLE_RATE / clientFormat.mSampleRate;
        
		
		// convert to our sample rate
		clientFormat.mSampleRate = LANTERN_AUDIO_SAMPLE_RATE;
		clientFormat.SetCanonical(LANTERN_AUDIO_NUM_CHANNELS, true); // TODO clientFormat.mChannelsPerFrame should be queried here.
		propSize = sizeof(clientFormat);
		ExtAudioFileSetProperty(xafRef, kExtAudioFileProperty_ClientDataFormat, propSize, &clientFormat);
		
		length = fileLengthFrames * LANTERN_AUDIO_NUM_CHANNELS * rateRatio;
		numChannels = LANTERN_AUDIO_NUM_CHANNELS; // TODO
	}
	@catch (NSException* exception) {
		if (buffer) free(buffer);
        if (doubleBuffer) free(doubleBuffer);
		buffer = NULL;
        doubleBuffer = NULL;
        fprintf(stdout, "AudioFileStream freeing data on bad constructor\n");
		length = loadedLengthCurrentBuffer = loadedLengthDoubleBuffer = numChannels = 0;
		rateRatio = 0;
		
		if (xafRef) ExtAudioFileDispose(xafRef);
		
        fprintf(stdout, "AudioFileStream constructor: asset file open and conversion failed: %s: %s\n", [exception.name UTF8String], [exception.reason UTF8String]);
        return false;
	}
	
	AudioConverterRef acRef;
	UInt32 acrSize = sizeof(AudioConverterRef);
	ExtAudioFileGetProperty(xafRef, kExtAudioFileProperty_AudioConverter, &acrSize, &acRef);
	
	AudioConverterPrimeInfo primeInfo;
	UInt32 piSize = sizeof(AudioConverterPrimeInfo);
	AudioConverterGetProperty(acRef, kAudioConverterPrimeInfo, &piSize, &primeInfo);
	numHeaderFrames = primeInfo.leadingFrames;
    
    xafRef_ptr = xafRef;
    return true;
}


// methods for manipulating our data and data pointer

// TODO
// this idx parameter is measured according to our own sample rate and number of channels.
// technically, ExtAudioFileSeek takes a parameter measured in the native specs for that file, which may not be the same.
// so this is missing some conversion.
bool AudioFileStream::seek(unsigned int idx) {
    ExtAudioFileRef xafRef = (ExtAudioFileRef)xafRef_ptr;
    if (xafRef) {
        @try {
            OSStatus error = ExtAudioFileSeek(xafRef, (SInt64)idx + numHeaderFrames);
            if (error) {
                fprintf(stdout, "AudioFileStream failed seek to idx %u\n", idx);
            } else
                fileIdx = idx;
        }
        @catch (NSException* exception) {
            fprintf(stdout, "AudioFileStream seek exception: %s: %s\n", [exception.name UTF8String], [exception.reason UTF8String]);
            return false;
        }
        
        xafRef_ptr = xafRef;
        return true;
    }
    // no file handle
    fileIdx = 0;
    return false;
}

bool AudioFileStream::loadSegment(unsigned int startIdx, unsigned int endIdx) {
	assert(startIdx >= 0);
	assert(endIdx < length);
    assert(endIdx > startIdx);
	
	seek(startIdx);
	return loadNextSegment(endIdx - startIdx);
}

bool AudioFileStream::loadNextSegment(unsigned int segmentLength) {
    if (segmentLength % numChannels != 0)
        segmentLength += (numChannels - (segmentLength % numChannels));
    
	unsigned int endIdx = fileIdx + segmentLength;
	if (endIdx > length) endIdx = length;
	
	@try {
		OSStatus error = noErr;
        if (useDoubleBuffering)
            swapBuffers();
        
        // if segment length has changed, we'll realloc
		unsigned int targetLength = (endIdx - fileIdx) * rateRatio;
		if (loadedLengthCurrentBuffer != targetLength) {
            if (buffer) {
                free(buffer);
                buffer = NULL;
            }
		}
        
        // zero segment length? unload
		if (targetLength == 0) {
			loadedLengthCurrentBuffer = 0;
            if (buffer) {
                free(buffer);
                buffer = NULL;
            }
			isLoaded = false;
		} else {
            // ok, we're ready to load the segment
            ExtAudioFileRef xafRef = (ExtAudioFileRef)xafRef_ptr;
            
			if (!buffer) {
				buffer = (Sample*) malloc(targetLength * sizeof(Sample));
                if (!buffer) {
                    fprintf(stdout, "AudioFileStream loadNextSegment alloc failed\n");
                    return false;
                }
                loadedLengthCurrentBuffer = targetLength;
            }
			
			// create an audio buffer and read in to it
			
			UInt32 numFrames = (UInt32)(targetLength / numChannels); // total frames to read; afterward, number of frames actually read
			AudioBufferList bufList;
			bufList.mNumberBuffers = 1;
			bufList.mBuffers[0].mNumberChannels = numChannels;
			bufList.mBuffers[0].mData = buffer;
			bufList.mBuffers[0].mDataByteSize = targetLength * sizeof(Sample);
			
			error = ExtAudioFileRead(xafRef, &numFrames, &bufList);
			if (error || numFrames == 0) {
                // graceful failure: let's just lie and say we read silence.
                memset(buffer, 0, targetLength * sizeof(Sample));
                seek(fileIdx + targetLength);
                loadedLengthCurrentBuffer = targetLength;
                isLoaded = true;
			} else {
				// success!
				isLoaded = true;
				buffer = (Sample*) (bufList.mBuffers[0].mData);
				loadedLengthCurrentBuffer = numFrames * numChannels;
                if (loadedLengthCurrentBuffer != targetLength) {
                    // interesting case; could print something
                }
				fileIdx += loadedLengthCurrentBuffer;
				// numChannels = bufList.mBuffers[0].mNumberChannels;
			}
            xafRef_ptr = xafRef;
		}
	}
	@catch (NSException* exception) {
		if (buffer) free(buffer);
        buffer = NULL;
        
		isLoaded = false;
		loadedLengthCurrentBuffer = 0;

		fprintf(stdout, "AudioFileStream loadNextSegment failed: %s: %s", [exception.name UTF8String], [exception.reason UTF8String]);
        return false;
	}
    
    return true;
}

void AudioFileStream::swapBuffers() {
    Sample* temp = doubleBuffer;
    doubleBuffer = buffer;
    buffer = temp;
    
    unsigned int tempLength = loadedLengthDoubleBuffer;
    loadedLengthDoubleBuffer = loadedLengthCurrentBuffer;
    loadedLengthCurrentBuffer = tempLength;
}
