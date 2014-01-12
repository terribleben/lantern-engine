//
//  ProcessorPan.cpp
//  lantern
//

#include "ProcessorPan.h"
#include <algorithm>

// TODO use rounder pan model. This one is totally linear.

ProcessorPan::ProcessorPan() : Processor() {
    setWidth(1.0f);
    setPan(0.5f);
}

ProcessorPan::~ProcessorPan() { }

void ProcessorPan::recompute() {
    
}

void ProcessorPan::setWidth(float width) {
    if (width < 0.0f) width = 0.0f;
    if (width > 1.0f) width = 1.0f;
    
    this->width = width;
    
    widthFullWeight = 0.5f + (0.5f * width);
    widthEmptyWeight = 0.5f - (0.5f * width);
}

void ProcessorPan::setPan(float panVal) {
    if (panVal < 0.0f) panVal = 0.0f;
    if (panVal > 1.0f) panVal = 1.0f;
    
    rightChannelWeight = std::min(1.0f, panVal * 2.0f);
    leftChannelWeight = std::min(1.0f, (1.0f - panVal) * 2.0f);
}

void ProcessorPan::getFrame(Sample* samples, long long frameId) {
    if (this->input)
        this->input->getFrameCached(samples, frameId);
    
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc += 2) {
        Sample leftVal = *samples, rightVal = *(samples + 1);
        process(leftVal, rightVal, samples, samples + 1);
        
        samples += 2;
    }
}

Sample ProcessorPan::process(Sample input) {
    // do nothing, override getFrame()
    return 0;
}

void ProcessorPan::process(Sample left, Sample right, Sample* outLeft, Sample* outRight) {
    // center general case:
    //    width = 0...1
    //    full = 0.5 + (0.5 * width), empty = 0.5 - (0.5 * width)
    //    left = (full * left + empty * right), right = (empty * left + full * right)
    Sample leftWidth = (widthFullWeight * left + widthEmptyWeight * right);
    Sample rightWidth = (widthEmptyWeight * left + widthFullWeight * right);
    
    // half left: left =  (1 * left + 0.5 * right), right = 0.5 * right
    // left: left = (1 * left + 1 * right), right = 0
    *outLeft = gain * (leftChannelWeight * leftWidth + (1.0f - rightChannelWeight) * rightWidth);
    *outRight = gain * ((1.0f - leftChannelWeight) * leftWidth + rightChannelWeight * rightWidth);
}
