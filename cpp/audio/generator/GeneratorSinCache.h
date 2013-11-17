//
//  GeneratorSinCache.h
//  Sine wave generator.
//  Internally uses a fixed-length sin cache and linear interpolation.
//
//  The SHM class is better, use that.
//  lantern
//

#ifndef __LANTERN_GENERATOR_SIN_CACHE_H__
#define __LANTERN_GENERATOR_SIN_CACHE_H__

#include "Generator.h"

class GeneratorSinCache : public Generator {
public:
    GeneratorSinCache();
    ~GeneratorSinCache();
    
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
