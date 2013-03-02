//
//  ResourceManager.h
//  The inner workings of this class are allowed to be platform dependent, but the interface should still be c++.
//  lantern
//

#ifndef __LANTERN_RESOURCE_MANAGER_H__
#define __LANTERN_RESOURCE_MANAGER_H__

#include <string.h>
#include <map>

#include "draw.h"

using std::string;
using std::map;

class ResourceManager {
public:
    static ResourceManager& getInstance();
    
    string fileContentsAsString(const char* path);
    void writeStringToFile(string str, const char* path);
    
    const char* prependResourcePath(const char* path);
    
    bool loadTexture(const char* name, const char* ext);
    bool loadTexture(const char* name, const char* ext, const char* key);
    
    bool bindTexture(const char* key);
    void unloadTexture(const char* key);
    
private:
    ResourceManager();
    ResourceManager(const ResourceManager& other);
    ~ResourceManager();
    
    map<string, GLuint> texMap;
};

#endif