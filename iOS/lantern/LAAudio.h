//
//  LAAudio.h
//
//  Takes care all the Apple AVAudio garbage needed to get the device hardware talking to an audio callback.
//  Sends interleaved 32-bit floating point samples.
//
//  lantern
//

#import <AudioUnit/AudioUnit.h>
#include "AudioDef.h"

#define LANTERN_AUDIO_VERBOSE 1

@interface LAAudio : NSObject

/**
 *  Construct a LAAudio object using a c-style callback.
 */
- (id) initWithSampleRate: (double)sampleRate
               bufferSize: (UInt16)bufferSize
                 callback: (AudioCallback)callback
                 userData: (void*)data;

- (BOOL) startSession;
- (BOOL) suspendSession;
- (BOOL) resumeSession;

@property (nonatomic, assign) BOOL overrideToSpeaker;
@property (nonatomic, assign) BOOL enableMic;

@property (nonatomic, readonly) double sampleRate;
@property (nonatomic, readonly) UInt16 bufferSize;
@property (nonatomic, readonly) BOOL hasMic;

@property (nonatomic, readwrite) AudioUnit audioUnit;

@end
