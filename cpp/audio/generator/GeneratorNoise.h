//
//  GeneratorNoise.h
//  lantern
//

#ifndef __LANTERN_GENERATOR_NOISE_H__
#define __LANTERN_GENERATOR_NOISE_H__

#include "Generator.h"

class GeneratorNoise : public Generator {
public:
    GeneratorNoise();
    virtual ~GeneratorNoise();
    
    virtual Sample next();
    
protected:
    void recompute();
};

#endif
