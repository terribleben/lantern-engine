//
//  Generator.h
//  Base class for a track that generates its own samples.
//  lantern
//

#ifndef __LANTERN_GENERATOR_H__
#define __LANTERN_GENERATOR_H__

#include "Track.h"

class Generator : public Track {
public:
    Generator();
    virtual ~Generator();
    
    virtual Sample next() = 0;
    virtual void getFrame(Sample* samples);
    virtual bool isFinished();
    
    virtual void setGain(float);
    virtual void setFreq(float); // bounded between 0.1 Hz and 20 kHz.
    void stop();
    
    float getFreq();
    
protected:
    bool isStopped;
    float freq;
    
    // called after setting a property of the generator
    // in case you need to recompute internal parameters.
    virtual void recompute() = 0;
};

#endif
