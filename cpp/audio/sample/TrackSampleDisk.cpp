/*
 *  TrackSampleDisk.cpp
 *  lantern
 */

#include "TrackSampleDisk.h"
#include <cstdlib>
#include <stdio.h>

TrackSampleDisk::TrackSampleDisk() : TrackSample() {
    fileStream = NULL;
	clear();
}

bool TrackSampleDisk::open(const char* filepath) {
    if (fileStream || isArmed)
        clear();
    
    fileStream = new AudioFileStream();
    if (fileStream->open(filepath)) {
        return true;
    } else {
        delete fileStream;
        fileStream = NULL;
        return false;
    }
}

void TrackSampleDisk::clear() {
    TrackSample::clear();
    
    if (fileStream) {
        delete fileStream;
        fileStream = NULL;
    }

	bufferLeftBound = bufferMiddleBound = bufferRightBound = 0;
    readError = false;
}

void TrackSampleDisk::arm() {
    if (!fileStream)
        return;

	fileStream->reset();
    fileStream->setEnableDoubleBuffering(true);
	
	bufferLeftBound = bufferMiddleBound = bufferRightBound = 0;
	
	fillBuffer(); // fill the audioData buffer
	fillBuffer(); // fill double buffer

	length = fileStream->getLength();
	idx = 0;
	idxFraction = 0;
	eof = false;

	isArmed = true;
    readError = false;
}

void TrackSampleDisk::fillBuffer() {
	fileStream->loadNextSegment(LANTERN_TRACK_WAV_SEGMENT_LENGTH);
	
	bufferLeftBound = bufferMiddleBound;
	bufferMiddleBound = bufferRightBound;
    
    unsigned int bytesRead = fileStream->getLoadedLength();
    if (bytesRead < 1) {
        readError = true;
        bufferRightBound += LANTERN_TRACK_WAV_SEGMENT_LENGTH; // we'll pad with 0s for awhile
    } else {
        readError = false;
        bufferRightBound += bytesRead;
    }
}

Sample TrackSampleDisk::sampleAt(unsigned int idx) {
	if (idx >= bufferMiddleBound)
		return fileStream->buffer[idx - bufferMiddleBound];
	if (idx >= bufferRightBound)
		return fileStream->buffer[bufferRightBound - bufferMiddleBound - 1];
	
	Sample s = fileStream->doubleBuffer[idx - bufferLeftBound];
	return s;
}

Sample TrackSampleDisk::next() {
    if (!isArmed || eof || !fileStream)
        return 0;
    else if (readError) {
        forward();
        return 0;
    }
    return TrackSample::next();
}

void TrackSampleDisk::forward() {
	if (eof) return;
    
    channelIdx++;
    if (channelIdx == LANTERN_AUDIO_NUM_CHANNELS) {
        idxFraction += tickSpeed;
        while (idxFraction >= 1) {
            idxFraction--;
            idx += fileStream->getNumChannels();
        }
        if (idx + idxFraction >= length) {
            eof = true;
            if (loopBehavior == LANTERN_AUDIO_WAV_LOOP_FOREVER)
                arm();
        } else if (idx >= bufferMiddleBound && bufferRightBound < length) {
            fillBuffer();
        }
        channelIdx = 0;
    }
}
