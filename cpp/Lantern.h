//
//  lantern.h
//  lantern
//

#ifndef __LANTERN_H__
#define __LANTERN_H__

#include <OpenGLES/ES1/gl.h>

#include "Event.h"
#include "View.h"
#include "AudioDef.h"
#include "Track.h"

#include <list>
using std::list;

class Lantern {
public:
    Lantern();
    virtual ~Lantern() {  }
    
    // Will return the most recent Lantern instance constructed (globally).
    // If you're using this library correctly, that's all you care about.
    static Lantern* getPrimaryInstance();
    

    // main graphics loop
    virtual void init();
    virtual void start();
    virtual void draw();
    virtual void stop();
    
    // display properties
    virtual void setDimensions(GLfloat width, GLfloat height);
    virtual void setScale(GLfloat scale);
    virtual void setOrientation(bool isPortrait);
    
    // event
    virtual void event(Event&);
    
    // views
    virtual void addView(View* view, string key);
    
    // audio
    void getAudioFrame(Sample* samples);
    
    void setAudioInputBuffer(unsigned int key, AudioSharedBuffer* buffer);
    AudioSharedBuffer* getAudioInputBuffer(unsigned int key);
    
    void setIsMicrophoneEnabled(bool);
    bool getIsMicrophoneEnabled();
    
protected:
    Lantern(const Lantern& other);
    static void setPrimaryInstance(Lantern* lantern);
    
    virtual void gameWillBegin();
    virtual void setInitialView(string initialViewKey);
    
    virtual void transitionView();
    
protected:
    bool isRunning;
    bool isMicrophoneEnabled;
    
    GLfloat screenWidth, screenHeight, screenScale;
    bool isPortrait;
    
    View* view;
    string initialViewKey;
    map<string, View*> views;
    
    list<Track*> audioTracks;
    map<unsigned int, AudioSharedBuffer*> inputBuffers;
    long long audioFrameId;
};

#endif
