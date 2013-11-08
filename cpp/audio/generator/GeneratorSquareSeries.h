//
//  GeneratorSquareSeries.h
//
//  Generates square waves that look more like an analogue circuit.
//  It gets closer to a perfect square wave, and also more expensive to compute, as you increase the number of harmonics.
//  In particular, there is a call to std::sin() for every harmonic for every sample.
//
//  lantern
//

#ifndef __LANTERN_GENERATOR_SQUARE_SERIES_H__
#define __LANTERN_GENERATOR_SQUARE_SERIES_H__

#include "Generator.h"

class GeneratorSquareSeries : public Generator {
public:
    GeneratorSquareSeries();
    ~GeneratorSquareSeries();
    
    Sample next();
    
    void setNumHarmonics(unsigned int);
    
private:
    void recompute();
    
    unsigned int step;
    unsigned int numHarmonics;
    float* innerCoefficients;
    float* outerCoefficients;
};

#endif
