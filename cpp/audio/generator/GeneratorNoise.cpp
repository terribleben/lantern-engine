//
//  GeneratorNoise.cpp
//  lantern
//

#include "GeneratorNoise.h"
#include "rand.h"

GeneratorNoise::GeneratorNoise() : Generator() {
    
}

GeneratorNoise::~GeneratorNoise() {
    
}

void GeneratorNoise::recompute() {
    
}

Sample GeneratorNoise::next() {
    return (-1.0f + 2.0f * lantern_randf()) * gain;
}
