//
//  AudioFilePool.h
//  For audio files that are small enough to reasonably store the whole thing in RAM,
//  we can keep a single copy in a pool, allowing multiple tracks to access that file at once.
//  lantern
//

#ifndef __LANTERN_AUDIO_FILE_POOL_H__
#define __LANTERN_AUDIO_FILE_POOL_H__

#include "AudioDef.h"
#include "AudioFileStream.h"

#include <map>
using std::map;

typedef struct AudioFileSharedBuffer {
    Sample* buffer;
    size_t length;
} AudioFileSharedBuffer;


class AudioFilePool {
public:
    static AudioFilePool& getInstance();
    
    bool load(const char* filename, const char* key);
    bool remove(const char* key);
    
    bool getData(const char* key, AudioFileSharedBuffer* sharedBuffer);
    
private:
    AudioFilePool();
    AudioFilePool(const AudioFilePool& other);
    ~AudioFilePool();
    
    map<const char*, AudioFileStream*> pool;
};

#endif
