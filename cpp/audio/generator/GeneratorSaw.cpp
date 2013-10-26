//
//  GeneratorSaw.cpp
//  lantern
//

#include "GeneratorSaw.h"

#include <stdio.h>

GeneratorSaw::GeneratorSaw() : Generator() {
    period = 0;
    phase = 0;
    magnitude = 0;
    
    setGain(0.5f);
    setFreq(220.0f);
}

GeneratorSaw::~GeneratorSaw() {
    
}

void GeneratorSaw::recompute() {
    // SAMPLE_RATE / freq = length of one period in samples
    // g / length of period = inc
    
    float oldPhaseFloat = (period) ? (float)phase / (float)period : 0.0f;
    
    baseline = -(gain * 0.5f);
    period = LANTERN_AUDIO_SAMPLE_RATE / freq;
    inc = gain / (float)period;
    
    phase = oldPhaseFloat * (float)period;
    magnitude = baseline + (inc * (float)phase);
}

Sample GeneratorSaw::next() {
    Sample result = magnitude;
    
    phase++;
    magnitude += inc;
    
    if (phase == period) {
        phase = 0;
        magnitude = baseline;
    }
    
    return result;
}
