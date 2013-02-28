//
//  ResourceManager.mm
//  lantern
//

#include <iostream>
#include <fstream>
#include "ResourceManager.h"

using std::ifstream;
using std::ofstream;
using std::istreambuf_iterator;

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager theRM;
    return theRM;
}

string ResourceManager::fileContentsAsString(const char *path) {
    ifstream ifs(path);
    if (!ifs.good()) {
        return "";
    }
    
    return string((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
}

void ResourceManager::writeStringToFile(string str, const char* path) {
    ofstream ofs(path);
    if (!ofs.good())
        return;
    
    if (!ofs.is_open())
        return;
    
    ofs << str;
    ofs.close();
}

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
    for (map<string, GLuint>::iterator it = texMap.begin(); it != texMap.end(); it++) {
        glDeleteTextures(1, &(it->second));
    }
    texMap.clear();
}

const char* ResourceManager::prependResourcePath(const char* path) {
    return [[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:[NSString stringWithUTF8String:path]] UTF8String];
}

bool ResourceManager::loadTexture(const char* name, const char* ext) {
    UIImage* image = nil;
    
    // attempt to use apple's cache and take advantage of a retina version
    if (strncmp(ext, "png", 3) == 0) {
        image = [[UIImage imageNamed:[NSString stringWithUTF8String:name]] retain];
    }
    
    // otherwise load it directly out of the bundle
    if (image == nil) {
        NSString* path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:name] ofType:[NSString stringWithUTF8String:ext]];
        image = [[UIImage alloc] initWithData:[NSData dataWithContentsOfFile:path]];
    }
    
    if (image == nil) {
        NSLog(@"ResourceManager failed to load UIImage from %s.%s", name, ext);
        return false;
    }
    
    // convert to RGBA
    GLuint width = CGImageGetWidth(image.CGImage);
    GLuint height = CGImageGetHeight(image.CGImage);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void* imageData = malloc(height * width * 4);
    CGContextRef context = CGBitmapContextCreate( 
                                                 imageData, width, height, 8, 4 * width, colorSpace, 
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease(colorSpace);
    CGContextClearRect(context, CGRectMake(0, 0, width, height));
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), image.CGImage);
    
    // load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    CGContextRelease(context);
    free(imageData);
    [image release];
    return true;
}

bool ResourceManager::loadTexture(const char* name, const char* ext, const char* key) {
    if (texMap.find(key) != texMap.end()) {
        // key already exists
        return false;
    }
    
    GLuint tex = 0;
    glGenTextures(1, &tex);
    
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    loadTexture(name, ext);
    
    texMap[key] = tex;
    
    return true;
}

bool ResourceManager::bindTexture(const char* key) {
    if (texMap.find(key) != texMap.end()) {
        glBindTexture(GL_TEXTURE_2D, texMap[key]);
        return true;
    }
    return false;
}