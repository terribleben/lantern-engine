//
//  GeneratorSquareSeries.cpp
//  lantern
//

#include <cstdlib>
#include "GeneratorSquareSeries.h"
#include "util_math.h"

GeneratorSquareSeries::GeneratorSquareSeries() : Generator() {
    innerCoefficients = NULL;
    outerCoefficients = NULL;
    step = 0;
    setNumHarmonics(1);
}

GeneratorSquareSeries::~GeneratorSquareSeries() {
    if (innerCoefficients) {
        free(innerCoefficients);
        innerCoefficients = NULL;
    }
    if (outerCoefficients) {
        free(outerCoefficients);
        outerCoefficients = NULL;
    }
}

void GeneratorSquareSeries::setNumHarmonics(unsigned int numHarmonics) {
    this->numHarmonics = numHarmonics;
    recompute();
}

void GeneratorSquareSeries::recompute() {
    if (innerCoefficients) {
        free(innerCoefficients);
        innerCoefficients = NULL;
    }
    if (outerCoefficients) {
        free(outerCoefficients);
        outerCoefficients = NULL;
    }
    
    innerCoefficients = (float*) malloc(sizeof(float) * numHarmonics);
    outerCoefficients = (float*) malloc(sizeof(float) * numHarmonics);
    
    // kth term of the sum at time t:
    // sin(2pi * (2k - 1) * ft) * (1 / (2k - 1)) * (4 / pi).
    //
    // inner coefficients are the stuff inside sin(); outer coefficients are all the other crap.
    
    float angularFrequency = M_2PI * (freq / LANTERN_AUDIO_SAMPLE_RATE);
    
    for (unsigned int kInt = 0; kInt < numHarmonics; kInt++) {
        float k = kInt + 1.0f;
        float sumCoefficient = (2.0f * k) - 1.0f;
        outerCoefficients[kInt] = 4.0f / (sumCoefficient * M_PI);
        innerCoefficients[kInt] = angularFrequency * sumCoefficient;
    }
}

Sample GeneratorSquareSeries::next() {
    Sample sum = 0;
    for (unsigned int k = 0; k < numHarmonics; k++) {
        sum += outerCoefficients[k] * sin(innerCoefficients[k] * step);
    }
    step++;
    return sum;
}
