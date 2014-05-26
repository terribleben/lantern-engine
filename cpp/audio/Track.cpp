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

void Track::getFrameCached(Sample* samples, long long frameId) {
    if (cachedFrameId == frameId) {
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            samples[cc] = cachedFrame[cc];
        }
    } else {
        getFrame(samples, frameId);
        for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
            cachedFrame[cc] = samples[cc];
        }
        cachedFrameId = frameId;
    }
}
