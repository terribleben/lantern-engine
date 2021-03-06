//
//  Processor.h
//  Base class for a track that reads input samples and outputs other samples (hopefully related).
//  lantern
//

#ifndef __LANTERN_PROCESSOR_H__
#define __LANTERN_PROCESSOR_H__

#include "Track.h"

class Processor : public Track {
public:
    Processor();
    virtual ~Processor();
    
    virtual Sample process(Sample input) = 0;
    virtual void getFrame(Sample* samples, long long frameId);
    virtual bool isFinished();
    void stop();
    
    virtual void setGain(float);
    virtual void setInput(Track*);
    virtual Track* getInput();
    
protected:
    Track* input;
    bool isStopped;
    
    // called after setting a property of the processor
    // in case you need to recompute internal parameters.
    virtual void recompute() = 0;
};

#endif
