//
//  ProcessorDelay.cpp
//  lantern
//

#include <cstdlib>
#include <string.h>
#include <math.h>
#include <algorithm>
#include "ProcessorDelay.h"

ProcessorDelay::ProcessorDelay() : Processor() {
    length = 0;
    bufferLength = 0;
    feedback = 0;
    buffer = NULL;
    bufferPointer = NULL;
}

ProcessorDelay::ProcessorDelay(unsigned int length, float feedback) {
    this->length = 0;
    bufferLength = 0;
    this->feedback = 0;
    buffer = NULL;
    bufferPointer = NULL;
    
    setLength(length);
    this->feedback = feedback;
}

ProcessorDelay::~ProcessorDelay() {
    if (buffer){
        free(buffer);
        buffer = NULL;
    }
}

void ProcessorDelay::recompute() {
    
}

void ProcessorDelay::setLength(float seconds) {
    unsigned int lengthFrames = (seconds * (float)LANTERN_AUDIO_SAMPLE_RATE);
    this->setLength(lengthFrames);
}

void ProcessorDelay::setLength(unsigned int length) {
    unsigned int lengthSamples = length * LANTERN_AUDIO_NUM_CHANNELS;
    
    // compute the necessary buffer size to accomodate this length
    unsigned int actualBufferAllocLength = (LANTERN_AUDIO_SAMPLE_RATE * LANTERN_AUDIO_NUM_CHANNELS) / 10;
    while (actualBufferAllocLength < lengthSamples) {
        actualBufferAllocLength *= 2;
    }
    
    if (actualBufferAllocLength > bufferLength) {
        // existing buffer is insufficient, realloc
        Sample* newBuffer = (Sample*) malloc(sizeof(Sample) * actualBufferAllocLength);
        if (buffer) {
            memcpy(newBuffer, buffer, sizeof(Sample) * std::min(bufferLength, actualBufferAllocLength));
            bufferPointer = newBuffer + (bufferPointer - buffer);
            if (bufferPointer > newBuffer + (length * LANTERN_AUDIO_NUM_CHANNELS))
                bufferPointer = newBuffer;
            
            free(buffer);
            buffer = NULL;
        } else {
            memset(newBuffer, 0, sizeof(Sample) * actualBufferAllocLength);
            bufferPointer = newBuffer;
        }
        
        buffer = newBuffer;
        bufferLength = actualBufferAllocLength;
    }
    
    this->length = length;
}

unsigned int ProcessorDelay::getLength() {
    return length;
}

Sample ProcessorDelay::process(Sample input) {
    Sample output = input + (*bufferPointer * feedback);
    
    *bufferPointer = output;
    bufferPointer++;
    if (bufferPointer >= buffer + (length * LANTERN_AUDIO_NUM_CHANNELS))
        bufferPointer = buffer;
    
    return output * gain;
}
