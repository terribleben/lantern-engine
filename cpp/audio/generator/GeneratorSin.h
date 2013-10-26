//
//  GeneratorSin.h
//  Sine wave generator.
//  Internally uses a fixed-length sin cache and linear interpolation.
//  lantern
//

#ifndef __LANTERN_GENERATOR_SIN_H__
#define __LANTERN_GENERATOR_SIN_H__

#include "Generator.h"

class GeneratorSin : public Generator {
public:
    GeneratorSin();
    ~GeneratorSin();
    
    Sample next();
    
protected:
    unsigned int cacheLength;
    Sample* cache;
    
    float phase;
    float phaseInc;
    
    void recompute();
    void generateCache();
};

#endif
