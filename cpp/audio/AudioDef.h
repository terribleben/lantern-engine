//
//  AudioDef.h
//  lantern
//

#ifndef __LANTERN_AUDIO_DEF_H__
#define __LANTERN_AUDIO_DEF_H__

typedef float Sample; // 32 bit

typedef void (*AudioCallback)(Sample* buffer, unsigned int numFrames, void* userData);

#define LANTERN_AUDIO_SAMPLE_RATE 44100.0
#define LANTERN_AUDIO_BUFFER_SIZE 512
#define LANTERN_AUDIO_NUM_CHANNELS 2

#endif
