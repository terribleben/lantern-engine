//
//  Processor.cpp
//  lantern
//

#include <cstdlib>
#include "Processor.h"

Processor::Processor() : Track() {
    input = NULL;
    isStopped = false;
}

Processor::~Processor() { }

void Processor::setInput(Track* input) {
    this->input = input;
    recompute();
}

void Processor::setGain(float gain) {
    this->gain = gain;
    recompute();
}

bool Processor::isFinished() {
    return isStopped;
}

void Processor::stop() {
    isStopped = true;
}

void Processor::getFrame(Sample* samples) {
    if (isStopped) {
        // silence
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            *samples++ = 0;
        }
    } else {
        // input
        this->input->getFrame(samples);
        
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            // output
            *samples = process(*samples);
            samples++;
        }
    }
}
