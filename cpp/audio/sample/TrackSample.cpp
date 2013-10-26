/*
 *  TrackSample.cpp
 *  lantern
 */

#include "TrackSample.h"
#include <cstdlib>
#include <stdio.h>

TrackSample::TrackSample() : Track() {
	clear();
}

TrackSample::~TrackSample() {
	clear();
}

void TrackSample::getFrame(Sample* samples) {
    for (int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++) {
        *samples++ = next();
    }
}

void TrackSample::clear() {
	isArmed = false;
	
	idx = 0;
	idxFraction = 0.0f;
	channelIdx = 0;
	length = 0;
	eof = true;
	tickSpeed = 1.0f;
	gain = 1.0f;
    loopBehavior = LANTERN_AUDIO_WAV_LOOP_NONE;
}

unsigned int TrackSample::getLength() {
    return length;
}

void TrackSample::setLoopBehavior(unsigned int behavior) {
    loopBehavior = behavior;
}

bool TrackSample::isFinished() {
    return eof;
}

Sample TrackSample::next() {
	if (!isArmed || eof)
		return 0;
    else {
		// interpolation
		float interpolatedSample;
		if (idxFraction == 0) {
			interpolatedSample = sampleAt(idx + channelIdx);
		} else {
			float lowerSample = sampleAt(idx + channelIdx);
			float upperSample = sampleAt(idx + channelIdx + LANTERN_AUDIO_NUM_CHANNELS);
			interpolatedSample = lowerSample + (upperSample - lowerSample) * idxFraction;
		}
		
		forward();
		return interpolatedSample * gain;
	}
}

void TrackSample::forward() {
	if (eof) return;
    
    channelIdx++;
    if (channelIdx == LANTERN_AUDIO_NUM_CHANNELS) {
        idxFraction += tickSpeed;
        while (idxFraction >= 1) {
            idxFraction--;
            idx += LANTERN_AUDIO_NUM_CHANNELS;
        }
        if (idx + idxFraction >= length) {
            eof = true;
            if (loopBehavior == LANTERN_AUDIO_WAV_LOOP_FOREVER)
                arm();
        }
        channelIdx = 0;
    }
}
