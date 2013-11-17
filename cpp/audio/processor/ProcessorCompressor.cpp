//
//  ProcessorCompressor.cpp
//  lantern
//

#include "ProcessorCompressor.h"
#include <cmath>

#define LANTERN_PROCESSOR_COMPRESSOR_HISTORY_LENGTH 441

ProcessorCompressor::ProcessorCompressor() : Processor() {
    wet = 0.0f;
    isAttacking = false;
    isLimiter = false;
    
    threshold = 1.0f;
    makeup = 0.0f;
    ratio = 1.0f;
    attackInc = 0;
    releaseInc = 0;
    rms = 0;
    rmsFrameSum = 0;
    rmsInnerSum = 0;
    
    setAttack(0);
    setRelease(0);
    
    positionInFrame = 0;
}

ProcessorCompressor::~ProcessorCompressor() {
    history.clear();
}

void ProcessorCompressor::setIsLimiter(bool isLimiter) {
    this->isLimiter = isLimiter;
}

void ProcessorCompressor::setRatio(Sample ratioSample) {
    if (ratioSample < 1.0f)
        ratioSample = 1.0f;
    
    this->ratio = ratioSample;
    this->isLimiter = false;
}

void ProcessorCompressor::setMakeup(Sample makeup) {
    this->makeup = makeup;
}

void ProcessorCompressor::setThreshold(Sample thresholdSample) {
    this->threshold = std::fabs(thresholdSample);
}

void ProcessorCompressor::setAttack(float milliseconds) {
    if (milliseconds == 0) {
        attackInc = 1.0f;
    } else {
        float numSamples = (float)LANTERN_AUDIO_SAMPLE_RATE * (milliseconds / 1000.0f);
        attackInc = (1.0f / numSamples);
    }
}

void ProcessorCompressor::setRelease(float milliseconds) {
    if (milliseconds == 0) {
        releaseInc = -1.0f;
    } else {
        float numSamples = (float)LANTERN_AUDIO_SAMPLE_RATE * (milliseconds / 1000.0f);
        releaseInc = (-1.0f / numSamples);
    }
}

/**
 *  Expect trigger to be positive (since it's RMS)
 */
Sample ProcessorCompressor::compress(Sample input, Sample trigger) {
    if (trigger <= threshold)
        return input;
    else {
        Sample sign = (input > 0) ? 1 : -1;
        if (isLimiter) {
            return std::min(std::fabs(input), threshold) * sign;
        } else {
            Sample difference = trigger - threshold;
            Sample compressed = threshold + (difference / ratio);
            return (std::fabs(input) - (trigger - compressed)) * sign;
        }
    }
}

Sample ProcessorCompressor::process(Sample input) {
    rmsFrameSum += input;
    positionInFrame++;
    
    if (positionInFrame == LANTERN_AUDIO_NUM_CHANNELS) {
        rmsFrameSum /= (float)LANTERN_AUDIO_NUM_CHANNELS;
    
        // compute signal RMS from all channels
        history.push_back((rmsFrameSum * rmsFrameSum) * (1.0f / LANTERN_PROCESSOR_COMPRESSOR_HISTORY_LENGTH));
        rmsInnerSum += history.back();
        
        if (history.size() > LANTERN_PROCESSOR_COMPRESSOR_HISTORY_LENGTH) {
            rmsInnerSum -= history.front();
            history.pop_front();
        }
        rms = std::sqrt(rmsInnerSum);
        
        // compute envelope state
        if (isAttacking) {
            wet += attackInc;
            if (wet >= 1.0f) {
                wet = 1.0f;
                isAttacking = false;
            }
        } else {
            if (rms > threshold) {
                isAttacking = true;
            } else if (wet > 0.0f) {
                wet += releaseInc;
                if (wet <= 0.0f)
                    wet = 0.0f;
            }
        }
        
        positionInFrame = 0;
        rmsFrameSum = 0;
    }
    
    // compress (or not)
    Sample output = 0;
    
    if (wet > 0.0f) {
        output = compress(input, rms);
        output = output * wet + input * (1.0f - wet);
    } else
        output = input;
    
    return gain * (output + makeup);
}
