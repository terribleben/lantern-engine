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
    
    gain = g;
}

void Track::setGainDb(float decibels) {
    setGain(powf(10.0f, decibels / 20.0f));
}
