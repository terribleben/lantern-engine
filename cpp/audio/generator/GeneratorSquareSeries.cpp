//
//  GeneratorSquareSeries.cpp
//  lantern
//

#include <cstdlib>
#include "GeneratorSquareSeries.h"
#include "util_math.h"
#include <cstdio>

GeneratorSquareSeries::GeneratorSquareSeries() : Generator() {
    harmonics = NULL;
    constant = (4.0f / M_PI);
    setNumHarmonics(1);
}

GeneratorSquareSeries::~GeneratorSquareSeries() {
    if (harmonics) {
        delete [] harmonics;
        harmonics = NULL;
    }
}

void GeneratorSquareSeries::setNumHarmonics(unsigned int numHarmonics) {
    this->numHarmonics = numHarmonics;
    
    if (harmonics) {
        delete [] harmonics;
        harmonics = NULL;
    }
    harmonics = new GeneratorSinSHM[numHarmonics];
    
    recompute();
}

void GeneratorSquareSeries::recompute() {
    // kth term of the sum at time t:
    // sin(2pi * (2k - 1) * ft) * (1 / (2k - 1)) * (4 / pi).
    // That last term is taken care of by a constant inside next().
    
    for (unsigned int kInt = 0; kInt < numHarmonics; kInt++) {
        float k = kInt + 1.0f;
        float sumCoefficient = (2.0f * k) - 1.0f;
        harmonics[kInt].setFreq(freq * sumCoefficient);
        harmonics[kInt].setGain(1.0f / sumCoefficient);
    }
}

Sample GeneratorSquareSeries::next() {
    Sample sum = 0;
    
    for (unsigned int k = 0; k < numHarmonics; k++) {
        sum += harmonics[k].next();
    }
    
    return sum * constant * gain;
}
