//
//  ProcessorBiquad.cpp
//  lantern
//

#include "ProcessorBiquad.h"

ProcessorBiquad::ProcessorBiquad() : Processor() {
    currentChannel = 0;
    a0 = a1 = a2 = b0 = b1 = b2 = 0; // TODO
    
    histories = new ProcessorBiquadHistory[LANTERN_AUDIO_NUM_CHANNELS];
}

ProcessorBiquad::~ProcessorBiquad() {
    delete [] histories;
}

void ProcessorBiquad::recompute() {
    // TODO
}

/**
 *  y[n] = (b0/a0) * x[n] + (b1/a0) * x[n-1] + (b2/a0) * x[n-2]
 *                        - (a1/a0) * y[n-1] - (a2/a0) * y[n-2]
 */
Sample ProcessorBiquad::process(Sample input) {
    ProcessorBiquadHistory* history = histories + currentChannel;
    
    Sample output = b0 * input + b1 * history->x1 + b2 * history->x2
                    - a1 * history->y1 - a2 * history->y2;
    
    history->x2 = history->x1;
    history->y2 = history->y1;
    history->x1 = input;
    history->y1 = output;
    
    currentChannel++;
    currentChannel %= LANTERN_AUDIO_NUM_CHANNELS;
    
    return output;
}
