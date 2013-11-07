//
//  GeneratorSinSHM.cpp
//  lantern
//

#include "GeneratorSinSHM.h"
#include "util_math.h"
#include <stdio.h>

GeneratorSinSHM::GeneratorSinSHM() : Generator() {
    displacement = 1;
    velocity = 0;
    acceleration = 0;
    accelCoefficient = 0;
}

GeneratorSinSHM::~GeneratorSinSHM() { }

void GeneratorSinSHM::setFreq(float freq) {
    Generator::setFreq(freq);
    
    // a(t) = -(2piF)^2 * x(t)
    float angularFrequency = M_2PI * (freq / LANTERN_AUDIO_SAMPLE_RATE);
    accelCoefficient = -powf(angularFrequency, 2.0f);
}

void GeneratorSinSHM::recompute() {
    
}

Sample GeneratorSinSHM::next() {
    acceleration = displacement * accelCoefficient;
    velocity += acceleration;
    displacement += velocity;
    
    return displacement * gain;
}
