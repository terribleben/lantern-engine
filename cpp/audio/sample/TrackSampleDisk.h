//
//  TrackSampleDisk.h
//  An audio file that is large enough that you'd rather stream it from disk than read it into RAM completely.
//  Uses an AudioFileStream internally.
//  lantern
//

#ifndef __LANTERN_TRACK_SAMPLE_DISK_H__
#define __LANTERN_TRACK_SAMPLE_DISK_H__

#include "TrackSample.h"
#include "AudioFileStream.h"

// maximum number of samples to keep in RAM at any instant.
// actually, that number is twice this, since this class uses double buffering.
#define LANTERN_TRACK_WAV_SEGMENT_LENGTH 442

class TrackSampleDisk : public TrackSample {
public:
	TrackSampleDisk();

public:
    bool open(const char* filepath); // won't load anything into memory until you call arm().
	void arm();
	void clear();
    
protected:
    // underlying file handle
    AudioFileStream* fileStream;
    bool readError;
	
protected:
    Sample next(); // get one sample and increment file or channel pointer
    void forward();
	void fillBuffer();
	Sample sampleAt(unsigned int idx);
	
	unsigned int bufferLeftBound, bufferMiddleBound, bufferRightBound;
};

#endif