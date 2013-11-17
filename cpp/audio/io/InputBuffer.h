//
//  InputBuffer.h
//
//  Reads a shared input buffer from the primary Lantern instance
//  And passes it through.
//
//  To get mic input, construct one of these pointing to LANTERN_INPUT_BUFFER_MICROPHONE.
//
//  lantern
//

#ifndef __LANTERN_INPUT_BUFFER_H__
#define __LANTERN_INPUT_BUFFER_H__

#include "Track.h"

class InputBuffer : public Track {
public:
    InputBuffer(unsigned int bufferKey);
    virtual ~InputBuffer();
    
    virtual void getFrame(Sample* samples);
    virtual bool isFinished();
    
protected:
    unsigned int bufferKey;
    AudioSharedBuffer* buffer;
    unsigned int position;
    unsigned int length;
    
    void getBuffer();
    Sample next();
};

#endif
