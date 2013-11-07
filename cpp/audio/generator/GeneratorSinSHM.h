//
//  GeneratorSinSHM.h
//
//  Alternative implementation of a sin generator that uses simple harmonic motion internally.
//
//  lantern
//

#ifndef __LANTERN_GENERATOR_SIN_SHM_H__
#define __LANTERN_GENERATOR_SIN_SHM_H__

#include "Generator.h"

class GeneratorSinSHM : public Generator {
public:
    GeneratorSinSHM();
    ~GeneratorSinSHM();
    
    void setFreq(float);
    Sample next();
    
protected:
    void recompute();
    
    float displacement, velocity, acceleration;
    float accelCoefficient;
};

#endif
