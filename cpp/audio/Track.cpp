//
//  Track.cpp
//  lantern
//

#include "Track.h"

float Track::getGain() {
    return gain;
}

void Track::setGain(float g) {
    if (g < 0.0f) g = 0.0f;
    if (g > 1.0f) g = 1.0f;
    
    gain = g;
}