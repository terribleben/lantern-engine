//
//  GeneratorSinSHM.cpp
//  lantern
//

#include "GeneratorSinSHM.h"
#include "util_math.h"

GeneratorSinSHM::GeneratorSinSHM() : Generator() {
    displacement = 1;
    velocity = 0;
    acceleration = 0;
    springConstant = 0;
}

void GeneratorSinSHM::setFreq(float freq) {
    Generator::setFreq(freq);
    
    // a(t) = -(2piF)^2 * x(t)
    springConstant = -powf(2.0f * M_PI * freq, 2.0f);
}

void GeneratorSinSHM::recompute() {
    
}

Sample GeneratorSinSHM::next() {
    acceleration = displacement * springConstant;
    velocity += acceleration;
    displacement += velocity;
    
    return displacement * gain;
}
