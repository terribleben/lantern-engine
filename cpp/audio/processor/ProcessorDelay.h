//
//  ProcessorDelay.h
//
//  A delay line.
//  Be aware that this uses RAM proportional to the length of the line
//  (e.g. a 1-second delay @ 2 channels 44100 sample rate = ~350kb).
//  Internally it may also alloc more memory than you ask for in order to avoid lots of reallocs.
//
//  lantern
//

#ifndef __LANTERN_PROCESSOR_DELAY_H__
#define __LANTERN_PROCESSOR_DELAY_H__

#include "Processor.h"

class ProcessorDelay : public Processor {
public:
    ProcessorDelay(unsigned int length, float feedback);
    ~ProcessorDelay();
    
    Sample process(Sample input);
    
    // length, in frames, of the delay line
    unsigned int getLength();
    void setLength(unsigned int);
    void setLength(float seconds);
    
    // 0 no feedback, 1 complete feedback
    float feedback;
    
protected:
    ProcessorDelay();
    void recompute();
    
    unsigned int length;
    unsigned int bufferLength;
    Sample* buffer;
    Sample* bufferPointer;
};

#endif
