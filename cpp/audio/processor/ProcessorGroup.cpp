//
//  ProcessorGroup.cpp
//  lantern
//

#include "ProcessorGroup.h"

ProcessorGroup::ProcessorGroup() : Processor() {
    inputs.clear();
}

ProcessorGroup::~ProcessorGroup() {
    // we don't own any of the memory inside this->inputs
}

Sample ProcessorGroup::process(Sample input) {
    // pass through
    return gain * input;
}

void ProcessorGroup::recompute() {
    // do nothing
}

void ProcessorGroup::getFrame(Sample* samples) {
    // input
    memset(samples, 0, LANTERN_AUDIO_NUM_CHANNELS * sizeof(Sample));
    Sample samplesSum[LANTERN_AUDIO_NUM_CHANNELS];
    for (list<Track*>::iterator itr = inputs.begin(); itr != inputs.end(); itr++) {
        Track* input = *itr;
        input->getFrame(samplesSum);
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            samples[cc] += samplesSum[cc];
        }
    }
    
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
        // output
        *samples = process(*samples);
        samples++;
    }
}

void ProcessorGroup::setInput(Track* track) {
    clearInputs();
    addInput(track);
}

void ProcessorGroup::clearInputs() {
    inputs.clear();
}

void ProcessorGroup::addInput(Track* track) {
    inputs.push_back(track);
}
