//
//  TrackSamplePooled.h
//  A track representing an audio file that is small enough to read completely into RAM.
//  Uses the shared audio pool, so you can have multiple of these accessing the same sound at once.
//  That means you need to put the sound in the pool manually before adding one of these.
//
//  Also it supports crossfading if you set it to loop.
//  lantern
//

#ifndef __LANTERN_TRACK_SAMPLE_POOLED_H__
#define __LANTERN_TRACK_SAMPLE_POOLED_H__

#include "TrackSample.h"
#include "AudioFilePool.h"

class TrackSamplePooled : public TrackSample {
public:
    TrackSamplePooled(const char* audioPoolKey);
    void setCrossfadeDuration(float seconds);
    
public:
    void arm();
    
protected:
    AudioFileSharedBuffer sharedBuffer;
    bool bufferIsValid;
    unsigned int crossfadeDurationSamples;
    
    virtual Sample sampleAt(unsigned int idx);
};

#endif
