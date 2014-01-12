//
//  ProcessorReverbSimple.cpp
//  lantern
//

#include "ProcessorReverbSimple.h"

ProcessorReverbSimple::ProcessorReverbSimple() : Processor() {
    // allpass => 3 parallel mutually prime combs => add gate
    biquad = new ProcessorBiquad();
    biquad->setAllpass(0.8f, 8000.0f);
    
    output = new ProcessorGroup();
    combDelays[0] = 113; combDelays[1] = 337; combDelays[2] = 1051;
    
    for (int ii = 0; ii < 3; ii++) {
        combs[ii] = new ProcessorDelay(combDelays[ii], 0.7f);
        combs[ii]->setInput(biquad);
        output->addInput(combs[ii]);
    }
    
    setMix(1.0f);
    setDecayT60(2.0f);
}

ProcessorReverbSimple::~ProcessorReverbSimple() {
    if (output) {
        output->stop();
        delete output;
        output = NULL;
    }
    
    if (combs[0]) {
        for (int ii = 0; ii < 3; ii++) {
            combs[ii]->stop();
            delete combs[ii];
            combs[ii] = NULL;
        }
    }
    
    if (biquad) {
        biquad->stop();
        delete biquad;
        biquad = NULL;
    }
}

void ProcessorReverbSimple::setDecayT60(float seconds) {
    float decaySamples = seconds * LANTERN_AUDIO_SAMPLE_RATE;
    for (int ii = 0; ii < 3; ii++) {
        // want 60 decibel dropoff in (seconds) seconds
        // -60 / 20 = -3
        combs[ii]->feedback = powf(10.0f, -3.0f * (combs[ii]->getLength() / decaySamples));
    }
}

void ProcessorReverbSimple::setMix(float mix) {
    if (mix < 0.0f) mix = 0.0f;
    if (mix > 1.0f) mix = 1.0f;
    
    wet = mix;
    dry = 1.0f - mix;
}

void ProcessorReverbSimple::setInput(Track* input) {
    biquad->setInput(input);
}

void ProcessorReverbSimple::recompute() {
    
}

void ProcessorReverbSimple::getFrame(Sample* samples, long long frameId) {
    Sample processed[LANTERN_AUDIO_NUM_CHANNELS];
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
        processed[cc] = samples[cc];
    }
    output->getFrameCached(processed, frameId);
    
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
        samples[cc] = (processed[cc] * wet + samples[cc] * dry);
    }
}

Sample ProcessorReverbSimple::process(Sample input) {
    // override getFrame
    return 0;
}
