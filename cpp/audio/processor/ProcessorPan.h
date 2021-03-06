//
//  ProcessorPan.h
//  Utility processor to change the pan of an input.
//  Assumes stereo. Beyond 2 channels, it interleaves.
//  lantern
//

#ifndef __LANTERN_PROCESSOR_GAIN_PAN_H__
#define __LANTERN_PROCESSOR_GAIN_PAN_H__

#include "Processor.h"

class ProcessorPan : public Processor {
public:
    ProcessorPan();
    ~ProcessorPan();
    
    void getFrame(Sample* samples, long long frameId);
    
    void setWidth(float width); // 0 mono, 1 stereo (default 1)
    void setPan(float pan); // 0 left channel, 1 right channel (default 0.5)
    Sample process(Sample);
    
protected:
    void recompute();

    float width;
    float leftChannelWeight, rightChannelWeight;
    float widthFullWeight, widthEmptyWeight;
    
    void process(Sample left, Sample right, Sample* outLeft, Sample* outRight);
};

#endif
