//
//  GeneratorSquare.cpp
//  lantern
//

#include "GeneratorSquare.h"

GeneratorSquare::GeneratorSquare() : Generator() {
    phase = 0;
    period = 0;
    halfPeriod = 0;
}

void GeneratorSquare::recompute() {
    float oldPhaseFloat = (period) ? (float)phase / (float)period : 0.0f;
    
    period = LANTERN_AUDIO_SAMPLE_RATE / freq;
    halfPeriod = period / 2;
    phase = oldPhaseFloat * (float)period;
}

Sample GeneratorSquare::next() {
    Sample result = gain * ((phase++ < halfPeriod) ? 1.0f : -1.0f);
    
    if (phase == period) {
        phase = 0;
    }
    return result;
}
