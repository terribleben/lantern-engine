//
//  InputBuffer.cpp
//  lantern
//

#include "InputBuffer.h"
#include "Lantern.h"

InputBuffer::InputBuffer(unsigned int bufferKey) : Track() {
    this->bufferKey = bufferKey;
    position = 0;
    length = 0;
    buffer = NULL;
    
    getBuffer();
}

InputBuffer::~InputBuffer() {
    
}

void InputBuffer::getBuffer() {
    buffer = Lantern::getPrimaryInstance()->getAudioInputBuffer(bufferKey);
    if (buffer) {
        length = buffer->length / sizeof(Sample);
    } else {
        length = 0;
    }
    position = 0;
}

Sample InputBuffer::next() {
    if (!buffer)
        getBuffer();
    
    if (buffer && length) {
        if (position == length) {
            getBuffer();
        }
        
        return buffer->buffer[position++] * gain;
    } else {
        return 0;
    }
}

void InputBuffer::getFrame(Sample *samples) {
    for (unsigned int cc = 0; cc < LANTERN_AUDIO_NUM_CHANNELS; cc++){
        *samples++ = next();
    }
}

bool InputBuffer::isFinished() {
    return false;
}
