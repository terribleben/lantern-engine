//
//  ProcessorEnvelope.cpp
//  lantern
//

#include "ProcessorEnvelope.h"

ProcessorEnvelope::ProcessorEnvelope() : Processor() {
    reset();
}

ProcessorEnvelope::ProcessorEnvelope(float attack, float decay, float sustain, float release) {
    reset();
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
    recompute();
}

ProcessorEnvelope::~ProcessorEnvelope() { }

void ProcessorEnvelope::reset() {
    isOn = 0;
    framePosition = 0;
    
    attack = decay = sustain = release = 0;
    attackInc = decayInc = releaseInc = 0;
    
    stage = kProcessorEnvelopeStageAttack;
    level = 0;
}

void ProcessorEnvelope::on() {
    // no isOn check, just reset every time
    isOn = true;
    stage = kProcessorEnvelopeStageAttack;
    level = 0;
}

void ProcessorEnvelope::off() {
    if (isOn) {
        isOn = false;
        stage = kProcessorEnvelopeStageRelease;
    }
}

void ProcessorEnvelope::setAttack(float seconds) {
    attack = seconds;
    recompute();
}

float ProcessorEnvelope::getAttack() {
    return attack;
}

void ProcessorEnvelope::setDecay(float seconds) {
    decay = seconds;
    recompute();
}

float ProcessorEnvelope::getDecay() {
    return decay;
}

void ProcessorEnvelope::setSustain(float level) {
    sustain = level;
    recompute();
}

void ProcessorEnvelope::setRelease(float seconds) {
    release = seconds;
    recompute();
}

void ProcessorEnvelope::recompute() {
    unsigned int framesAttack = attack * LANTERN_AUDIO_SAMPLE_RATE;
    attackInc = 1.0f / (float)framesAttack;
    
    unsigned int framesDecay = decay * LANTERN_AUDIO_SAMPLE_RATE;
    decayInc = (sustain - 1.0f) / (float)framesDecay;
    
    unsigned int framesRelease = release * LANTERN_AUDIO_SAMPLE_RATE;
    releaseInc = -sustain / (float)framesRelease;
}

Sample ProcessorEnvelope::process(Sample input) {
    framePosition++;
    
    if (framePosition == LANTERN_AUDIO_NUM_CHANNELS) {
        // move forward in the envelope
        framePosition = 0;
        
        if (isOn) {
            if (stage == kProcessorEnvelopeStageAttack) {
                level += attackInc;
                if (level >= 1.0f) {
                    level = 1.0f;
                    stage = kProcessorEnvelopeStageDecay;
                }
            } else if (stage == kProcessorEnvelopeStageDecay) {
                level += decayInc;
                if (level <= sustain) {
                    level = sustain;
                    stage = kProcessorEnvelopeStageSustain;
                }
            }
        } else {
            // release or idle
            if (stage == kProcessorEnvelopeStageRelease) {
                level += releaseInc;
                if (level <= 0.0f) {
                    level = 0;
                    stage = kProcessorEnvelopeStageAttack;
                }
            }
        }
    }
    
    return input * level * gain;
}
