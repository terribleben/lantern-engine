//
//  TrackSamplePooled.cpp
//  lantern
//

#include "TrackSamplePooled.h"

TrackSamplePooled::TrackSamplePooled(const char* audioPoolKey) {
    bufferIsValid = (AudioFilePool::getInstance().getData(audioPoolKey, &sharedBuffer));
    crossfadeDurationSamples = 0;
}

void TrackSamplePooled::arm() {
    if (bufferIsValid) {
        length = sharedBuffer.length;
        idx = 0;
        idxFraction = 0;
        eof = false;
        isArmed = true;
        
        if (crossfadeDurationSamples)
            idx = crossfadeDurationSamples;
    } else
        isArmed = false;
}

void TrackSamplePooled::setCrossfadeDuration(float seconds) {
    unsigned int durationSamples = (seconds * LANTERN_AUDIO_SAMPLE_RATE * LANTERN_AUDIO_NUM_CHANNELS);
    if (durationSamples < length * 0.5f)
        crossfadeDurationSamples = durationSamples;
}

Sample TrackSamplePooled::sampleAt(unsigned int idx) {
    if (idx < length) {
        if (!crossfadeDurationSamples || (crossfadeDurationSamples && idx < length - crossfadeDurationSamples)) // no crossfade
            return sharedBuffer.buffer[idx];
        else {
            // linear interpolation
            unsigned int crossfadeIdx = idx - (length - crossfadeDurationSamples);
            Sample current = sharedBuffer.buffer[idx];
            Sample wrap = sharedBuffer.buffer[crossfadeIdx];
            
            float interp = (float)crossfadeIdx / (float)crossfadeDurationSamples;
            return (current * (1.0f - interp)) + (wrap * interp);
        }
    }
    return 0;
}
