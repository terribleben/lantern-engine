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

Track* Processor::getInput() {
    return this->input;
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

void Processor::getFrame(Sample* samples, long long frameId) {
    if (isStopped) {
        // silence
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            *samples++ = 0;
        }
    } else {
        // input
        this->input->getFrameCached(samples, frameId);
        
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            // output
            *samples = process(*samples);
            samples++;
        }
    }
}
