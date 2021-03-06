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

Sample ProcessorGroup::process(Sample inputSamp) {
    // pass through
    return gain * inputSamp;
}

void ProcessorGroup::recompute() {
    // do nothing
}

void ProcessorGroup::getFrame(Sample* samples, long long frameId) {
    // input
    memset(samples, 0, LANTERN_AUDIO_NUM_CHANNELS * sizeof(Sample));
    Sample samplesSum[LANTERN_AUDIO_NUM_CHANNELS];
    for (list<Track*>::iterator itr = inputs.begin(); itr != inputs.end(); itr++) {
        Track* input = *itr;
        input->getFrameCached(samplesSum, frameId);
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

void ProcessorGroup::removeInput(Track* track) {
    list<Track*>::iterator itr = inputs.begin();
    bool found = false;
    
    while (!found && itr != inputs.end()) {
        if (*itr == track) {
            inputs.erase(itr++);
            found = true;
        } else
            itr++;
    }
}
