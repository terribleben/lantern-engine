//
//  GeneratorSquareSeries.h
//
//  Generates square waves using a series of sin harmonics (so more like an actual circuit).
//  It gets closer to a perfect square wave, and also more expensive to compute, as you increase the number of harmonics.
//
//  lantern
//

#ifndef __LANTERN_GENERATOR_SQUARE_SERIES_H__
#define __LANTERN_GENERATOR_SQUARE_SERIES_H__

#include "Generator.h"
#include "GeneratorSinSHM.h"

class GeneratorSquareSeries : public Generator {
public:
    GeneratorSquareSeries();
    ~GeneratorSquareSeries();
    
    Sample next();
    
    void setNumHarmonics(unsigned int);
    
private:
    void recompute();
    
    unsigned int numHarmonics;
    GeneratorSinSHM* harmonics;
    float constant;
};

#endif
