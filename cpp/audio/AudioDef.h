//
//  AudioDef.h
//  lantern
//

#ifndef __LANTERN_AUDIO_DEF_H__
#define __LANTERN_AUDIO_DEF_H__

#include <cstdlib>
#include <complex>

typedef float Sample; // 32 bit

typedef void (*AudioCallback)(Sample* buffer, unsigned int numFrames, void* userData);

#define LANTERN_AUDIO_SAMPLE_RATE 44100.0
#define LANTERN_AUDIO_BUFFER_SIZE 512
#define LANTERN_AUDIO_MAX_BUFFER_SIZE 1024
#define LANTERN_AUDIO_NUM_CHANNELS 2

#define LANTERN_INPUT_BUFFER_MICROPHONE 1

typedef struct AudioSharedBuffer {
    Sample* buffer;
    size_t length;
} AudioSharedBuffer;

typedef std::complex<Sample> AudioComplex;

#endif
