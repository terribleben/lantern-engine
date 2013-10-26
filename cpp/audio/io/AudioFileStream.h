//
//  AudioFileStream.h
//  Handle to an audio file on disk.
//  Will only put as much data in RAM as you ask it to.
//  Supports double buffering for smoother playback.
//
//  This is not cross-platform since it relies on streaming stuff from disk.
//
//  lantern
//

#ifndef __LANTERN_AUDIO_FILE_STREAM_H__
#define __LANTERN_AUDIO_FILE_STREAM_H__

#include "AudioDef.h"

class AudioFileStream {
public:
    AudioFileStream();
    ~AudioFileStream();
    
public:
    // direct access to loaded samples
    Sample* buffer;
    Sample* doubleBuffer;
    
    bool open(const char* filepath); // get a handle, but don't load any data
    void reset(); // seek to beginning
    void clear(); // destroy file handle and unload buffers
    bool loadNextSegment(unsigned int segmentLength);
    
    void setEnableDoubleBuffering(bool enable);
    bool getIsLoaded();
    unsigned int getLoadedLength();
    unsigned int getLength();
    unsigned int getNumChannels();
    
protected:
    bool seek(unsigned int idx);
    void swapBuffers();
    bool loadSegment(unsigned int startIdx, unsigned int endIdx);
    
protected:
    bool isLoaded;
	unsigned int length;
	unsigned int numChannels;
    
    // buffer stuff
    bool useDoubleBuffering;
    unsigned int loadedLengthCurrentBuffer;
    unsigned int loadedLengthDoubleBuffer;
	
    // file handle stuff
	void* xafRef_ptr; // ExtAudioFileRef
	signed long long fileLengthFrames;
	signed long long fileIdx;
	signed long long numHeaderFrames;
	double rateRatio;
};

#endif
