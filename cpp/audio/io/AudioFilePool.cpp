//
//  AudioFilePool.cpp
//  lantern
//

#include "AudioFilePool.h"
#include "ResourceManager.h"

AudioFilePool& AudioFilePool::getInstance() {
    static AudioFilePool thePool;
    return thePool;
}

AudioFilePool::AudioFilePool() {
    
}

AudioFilePool::~AudioFilePool() {
    for (map<const char*, AudioFileStream*>::iterator it = pool.begin(); it != pool.end(); it++) {
        AudioFileStream* handle = it->second;
        delete handle;
    }
    pool.clear();
}

bool AudioFilePool::load(const char* filename, const char* key) {
    const char* filepath = ResourceManager::getInstance().prependResourcePath(filename);
    if (pool.count(key) == 0) {
        AudioFileStream* handle = new AudioFileStream();
        if (handle->open(filepath)) {
            // load the whole damn thing into RAM.
            if (handle->loadNextSegment(handle->getLength())) {
                pool[key] = handle;
                return true;
            }
        }
        
        // failed
        delete handle;
        return false;
    }
    // already exists
    return false;
}

bool AudioFilePool::remove(const char* key) {
    if (pool.count(key)) {
        AudioFileStream* handle = pool[key];
        delete handle;
        pool.erase(pool.find(key));
        return true;
    }
    // never existed to begin with
    return false;
}

bool AudioFilePool::getData(const char* key, AudioFileSharedBuffer* sharedBuffer) {
    if (pool.count(key)) {
        AudioFileStream* handle = pool[key];
        sharedBuffer->buffer = handle->buffer;
        sharedBuffer->length = handle->getLength();
        return true;
    }
    // not found
    return false;
}
