//
//  ProcessorReverbSimple.h
//
//  Mono => mono reverb processor. One allpass filter and three parallel comb filters.
//
//  lantern
//

#ifndef __LANTERN_PROCESSOR_REVERB_SIMPLE_H__
#define __LANTERN_PROCESSOR_REVERB_SIMPLE_H__

#include "Processor.h"
#include "ProcessorBiquad.h"
#include "ProcessorDelay.h"
#include "ProcessorGroup.h"

class ProcessorReverbSimple : public Processor {
public:
    ProcessorReverbSimple();
    ~ProcessorReverbSimple();
    
    // set the T60 decay time in seconds.
    void setDecayT60(float seconds);
    
    // 0 = dry signal, 1 = processed signal
    void setMix(float mix);
    
    virtual void getFrame(Sample*, long long frameId);
    void setInput(Track*);
    
protected:
    Sample process(Sample);
    void recompute();
    
    ProcessorBiquad* biquad;
    ProcessorDelay* combs[3];
    ProcessorGroup* output;
    
    unsigned int combDelays[3];
    float wet, dry;
};

#endif
