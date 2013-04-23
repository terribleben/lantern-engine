//
//  Track.h
//  Base class for all things that produce sound.
//  lantern
//

#ifndef __LANTERN_TRACK_H__
#define __LANTERN_TRACK_H__

#include "AudioDef.h"

class Track {
public:
    Track() : gain(1.0f) {  }
    virtual ~Track() {  }
    
    virtual void getFrame(Sample* samples) = 0;
    virtual bool isFinished() = 0;
    
    virtual void setGain(float); // bounded between 0 and 1.
    float getGain();
    
protected:
    float gain;
};

#endif
