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
    Lantern() : screenWidth(0), screenHeight(0), screenScale(1), view(NULL), isPortrait(false) {  }
    virtual ~Lantern() {  }

    // main graphics loop
    virtual void init();
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
    
protected:
    Lantern(const Lantern& other);
    
    virtual void gameWillBegin();
    virtual void setInitialView(string initialViewKey);
    
    virtual void transitionView();
    
protected:
    bool isRunning;
    
    GLfloat screenWidth, screenHeight, screenScale;
    bool isPortrait;
    
    View* view;
    string initialViewKey;
    map<string, View*> views;
    
    list<Track*> audioTracks;
};

#endif
