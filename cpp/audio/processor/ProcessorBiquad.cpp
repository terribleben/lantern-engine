//
//  ProcessorBiquad.cpp
//  lantern
//

#include "ProcessorBiquad.h"
#include "util_math.h"

ProcessorBiquad::ProcessorBiquad() : Processor() {
    currentChannel = 0;
    
    a0 = b0 = 1;
    a1 = a2 = b1 = b2 = 0;
    
    histories = new ProcessorBiquadHistory[LANTERN_AUDIO_NUM_CHANNELS];
}

ProcessorBiquad::~ProcessorBiquad() {
    delete [] histories;
}

void ProcessorBiquad::recompute() {
    // TODO
    // can probably incorporate gain into coefficients here instead of dealing with it in process()
}

void ProcessorBiquad::setCoefficients(Sample a0, Sample a1, Sample a2, Sample b0, Sample b1, Sample b2) {
    this->a0 = a0;
    this->a1 = a1 / a0;
    this->a2 = a2 / a0;
    this->b0 = b0 / a0;
    this->b1 = b1 / a0;
    this->b2 = b2 / a0;
}

void ProcessorBiquad::setPole(float radius, float frequency) {
    a2 = (radius * radius);
    a1 = (-2.0f * radius * std::cos(M_2PI * (frequency / LANTERN_AUDIO_SAMPLE_RATE)));
    
    // constant gain
    b0 = 0.5f - (0.5f * a2);
    b1 = 0;
    b2 = -b0;
    
    a1 /= a0; a2 /= a0; b0 /= a0; b1 /= a0; b2 /= a0;
}

void ProcessorBiquad::setZero(float radius, float frequency) {
    b2 = (radius * radius) / a0;
    b1 = (-2.0f * radius * std::cos(M_2PI * (frequency / LANTERN_AUDIO_SAMPLE_RATE))) / a0;
}

void ProcessorBiquad::setAllpass(float radius, float frequency) {
    // from wikipedia allpass filter
    // given a pole z0,
    // radius2 = |z0|^2
    // R = real component of z0
    // y[k] = radius2 * x[k]  - 2R * x[k - 1] + x[k - 2] + 2R * y[k - 1] - radius2 * y[k - 2]
    setPole(radius, frequency);
    b0 = a2;
    b1 = a1;
    b2 = 1.0f / a0;
}

/**
 *  y[n] = (b0/a0) * x[n] + (b1/a0) * x[n-1] + (b2/a0) * x[n-2]
 *                        - (a1/a0) * y[n-1] - (a2/a0) * y[n-2]
 */
Sample ProcessorBiquad::process(Sample input) {
    ProcessorBiquadHistory* history = histories + currentChannel;
    
    Sample output = b0 * input + b1 * history->x1 + b2 * history->x2
                    - a1 * history->y1 - a2 * history->y2;
    
    history->x2 = history->x1;
    history->y2 = history->y1;
    history->x1 = input;
    history->y1 = output;
    
    currentChannel++;
    currentChannel %= LANTERN_AUDIO_NUM_CHANNELS;
    
    return output * gain;
}
