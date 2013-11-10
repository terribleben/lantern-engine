//
//  Processor.cpp
//  lantern
//

#include <cstdlib>
#include "Processor.h"

Processor::Processor() : Track() {
    input = NULL;
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
    return false;
}

void Processor::getFrame(Sample* samples) {
    // input
    this->input->getFrame(samples);
    
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
        // output
        *samples = process(*samples);
        samples++;
    }
}
