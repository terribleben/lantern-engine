//
//  Generator.cpp
//  lantern
//

#include "Generator.h"

Generator::Generator() : Track() {
    freq = 20.0f;
    gain = 0.5f;
    isStopped = false;
}

Generator::~Generator() {
    
}

void Generator::setGain(float g) {
    Track::setGain(g);
    recompute();
}

void Generator::setFreq(float f) {
    if (f <= 0.1f) f = 0.1f;
    if (f > 20000.0f) f = 20000.0f;
    
    freq = f;
    recompute();
}

float Generator::getFreq() {
    return freq;
}

void Generator::stop() {
    isStopped = true;
}

void Generator::getFrame(Sample* samples) {
    // mono output of our next sample.
    Sample s = next();
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
        *samples++ = s;
    }
}

bool Generator::isFinished() {
    return isStopped;
}
