//
//  GeneratorSin.cpp
//  lantern
//

#include <cstdlib>
#include <cmath>
#include "GeneratorSin.h"

GeneratorSin::GeneratorSin() : Generator() {
    phase = 0.0f;
    phaseInc = 0.0f;
    
    cache = NULL;
    generateCache();
    
    setGain(0.5f);
    setFreq(220.0f);
}

GeneratorSin::~GeneratorSin() {
    if (cache) {
        free(cache);
        cache = NULL;
    }
}

void GeneratorSin::recompute() {
    phaseInc = freq / (float)LANTERN_AUDIO_SAMPLE_RATE;
}

void GeneratorSin::generateCache() {
    // the higher this number, the higher resolution the sin is.
    // uses (cacheLength * sizeof(Sample)) bytes of memory.
    cacheLength = 128;
    
    if (cache) {
        free(cache);
    }
    cache = (Sample*) malloc(sizeof(Sample) * cacheLength);
    
    float angle = 0.0f;
    float angleInc = M_PI * 2.0f * (1.0f / (float)cacheLength);

    Sample* cachePtr = cache;
    for (int ss = 0; ss < cacheLength; ss++) {
        *cachePtr++ = std::sin(angle);
        angle += angleInc;
    }
}

Sample GeneratorSin::next() {
    unsigned int idxLower = cacheLength * phase;
    unsigned int idxUpper = (idxLower + 1) % cacheLength;
    float interp = ((float)cacheLength * phase) - idxLower;
    
    float magnitude = ((cache[idxLower] * (1.0f - interp)) + (cache[idxUpper] * interp)) * gain;
    
    phase += phaseInc;
    if (phase >= 1.0f)
        phase = 0.0f;
    
    return magnitude;
}
