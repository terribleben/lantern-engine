//
//  ProcessorCompressor.h
//
//  Really simple compressor with a hard knee and RMS triggering.
//
//  lantern
//

#ifndef __LANTERN_PROCESSOR_COMPRESSOR_H__
#define __LANTERN_PROCESSOR_COMPRESSOR_H__

#include "Processor.h"

class ProcessorCompressor : public Processor {
public:
    ProcessorCompressor();
    ~ProcessorCompressor();
    
    Sample process(Sample input);
    
    void setAttack(float milliseconds);
    void setRelease(float milliseconds);
    
    void setRatio(Sample ratioSample); // set the ratio; automatically turns off isLimiter.
    void setIsLimiter(bool); // set the ratio to infinity.
    
    void setThreshold(float threshold); // in decibels; expected to be negative.
    void setMakeup(float makeup); // output amplitude is increased by (makeup) decibels
    
protected:
    Sample compress(Sample input, Sample trigger);
    void recompute();
    
    float wet; // 0 = inactive, 1 = fully active
    bool isAttacking;
    bool isLimiter;
    
    Sample* history;
    unsigned int historyIdx;
    
    Sample rmsFrameSum;
    Sample rmsInnerSum;
    Sample rms;
    Sample rmsTrigger;
    
    Sample threshold;
    Sample makeup;
    Sample ratio;
    Sample attackInc, releaseInc;
    
    unsigned int positionInFrame;
};

#endif
