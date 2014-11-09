//
//  TrackSample.h
//  Base class for a track that gets its samples from a sound file.
//  Can be stretched by changing its tick speed. (for now that must be positive.)
//  lantern
//

#ifndef __LANTERN_TRACK_SAMPLE_H__
#define __LANTERN_TRACK_SAMPLE_H__

#include "Track.h"
#include "AudioFileStream.h"

#define LANTERN_AUDIO_WAV_LOOP_NONE 0
#define LANTERN_AUDIO_WAV_LOOP_FOREVER 1

class TrackSample : public Track {
public:
	TrackSample();
	virtual ~TrackSample();
    
    void getFrame(Sample* samples);
    bool isFinished();
    
public:
    virtual void arm() = 0; // load any necessary data into RAM and prepare to play.
    virtual void clear(); // delete all owned audio data and become empty.
    virtual unsigned long getLength();
    void setLoopBehavior(unsigned int behavior);
    float tickSpeed;
    
protected:
    // position in buffer
	unsigned int idx;
	float idxFraction;
	unsigned int channelIdx;
    
    unsigned long length;
	bool eof;
    bool isArmed;
    
    unsigned int loopBehavior;
	
protected:
    virtual Sample next(); // get one sample and increment file or channel pointer
    virtual void forward();
	virtual Sample sampleAt(unsigned int idx) = 0;
};

#endif