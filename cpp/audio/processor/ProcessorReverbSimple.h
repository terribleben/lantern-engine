//
//  ProcessorReverbSimple.h
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
    
    virtual void getFrame(Sample*, long long frameId);
    void setInput(Track*);
    
protected:
    Sample process(Sample);
    void recompute();
    
    ProcessorBiquad* biquad;
    ProcessorDelay* combs[3];
    ProcessorGroup* output;
};

#endif
