//
//  ProcessorBiquad.h
//
//  Multichannel biquad filter.
//
//  lantern
//

#ifndef __LANTERN_PROCESSOR_BIQUAD_H__
#define __LANTERN_PROCESSOR_BIQUAD_H__

#include "Processor.h"

typedef struct ProcessorBiquadHistory {
    ProcessorBiquadHistory() : x1(0), x2(0), y1(0), y2(0) { }
    Sample x1, x2, y1, y2;
} ProcessorBiquadHistory;

class ProcessorBiquad : public Processor {
public:
    ProcessorBiquad();
    ~ProcessorBiquad();
    
    void setCoefficients(Sample a0, Sample a1, Sample a2, Sample b0, Sample b1, Sample b2);
    void setPole(float radius, float frequency);
    void setZero(float radius, float frequency);
    
    Sample process(Sample input);
    
protected:
    void recompute();
    
    // biquad coefficients
    Sample a0, a1, a2, b0, b1, b2;
    
    // biquad in/out history
    // (1 per channel)
    ProcessorBiquadHistory* histories;
    unsigned int currentChannel;
};

#endif
