//
//  GeneratorSaw.h
//  Saw wave generator.
//  lantern
//

#ifndef __LANTERN_GENERATOR_SAW_H__
#define __LANTERN_GENERATOR_SAW_H__

#include "Generator.h"

class GeneratorSaw : public Generator {
public:
    GeneratorSaw();
    ~GeneratorSaw();
    
    Sample next();
    
protected:
    Sample baseline;
    Sample inc;
    
    unsigned int phase;
    unsigned int period;
    Sample magnitude;
    
    void recompute();
};

#endif
