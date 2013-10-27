//
//  lantern.h
//  lantern
//

#ifndef __LANTERN_H__
#define __LANTERN_H__

#include <OpenGLES/ES1/gl.h>

#include "Event.h"
#include "View.h"

class Lantern {
public:
    Lantern() : screenWidth(0), screenHeight(0), screenScale(1), view(NULL), isPortrait(false) {  }
    ~Lantern() {  }

    // main graphics loop
    void init();
    void draw();
    void stop();
    
    // display properties
    void setDimensions(GLfloat width, GLfloat height);
    void setScale(GLfloat scale);
    void setOrientation(bool isPortrait);
    
    // event
    void event(Event&);
    
    // views
    void addView(View* view, string key);
    
protected:
    Lantern(const Lantern& other);
    
    void gameWillBegin();
    void setInitialView(string initialViewKey);
    
    void transitionView();
    
protected:
    bool isRunning;
    
    GLfloat screenWidth, screenHeight, screenScale;
    bool isPortrait;
    
    View* view;
    string initialViewKey;
    map<string, View*> views;
};

#endif
