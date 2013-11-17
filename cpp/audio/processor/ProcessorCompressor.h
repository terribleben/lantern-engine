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
#include <list>
using std::list;

class ProcessorCompressor : public Processor {
public:
    ProcessorCompressor();
    ~ProcessorCompressor();
    
    Sample process(Sample input);
    
    void setAttack(float milliseconds);
    void setRelease(float milliseconds);
    
    void setRatio(Sample ratioSample); // set the ratio. TODO make this operate in decibels. Automatically turns off isLimiter.
    void setIsLimiter(bool); // set the ratio to infinity.
    
    void setThreshold(Sample thresholdSample); // TODO make this accept decibels from zero.
    void setMakeup(Sample makeup); // added to the output
    
protected:
    Sample compress(Sample input, Sample trigger);
    void recompute();
    
    float wet; // 0 = inactive, 1 = fully active
    bool isAttacking;
    bool isLimiter;
    
    list<Sample> history;
    Sample rmsFrameSum;
    Sample rmsInnerSum;
    Sample rms;
    
    Sample threshold;
    Sample makeup;
    Sample ratio;
    Sample attackInc, releaseInc;
    
    unsigned int positionInFrame;
};

#endif
