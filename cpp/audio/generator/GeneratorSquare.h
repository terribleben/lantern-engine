//
//  GeneratorSquare.h
//  Perfect square wave generator.
//  For something closer to an analogue circuit, see GeneratorSquareSeries.
//
//  lantern
//

#ifndef __LANTERN_GENERATOR_SQUARE_H__
#define __LANTERN_GENERATOR_SQUARE_H__

#include "Generator.h"

class GeneratorSquare : public Generator {
public:
    GeneratorSquare();
    ~GeneratorSquare();
    
    Sample next();
    
private:
    void recompute();
    
    unsigned int phase;
    unsigned int period, halfPeriod;
};

#endif
