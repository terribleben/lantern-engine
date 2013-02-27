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
    static Lantern& getInstance();
    
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
    
private:
    Lantern() : screenWidth(0), screenHeight(0), screenScale(1), view(NULL), isPortrait(false) {  }
    Lantern(const Lantern& other);
    ~Lantern() {  }
    
    void transitionView();
    
private:
    GLfloat screenWidth, screenHeight, screenScale;
    bool isPortrait;
    View* view;
};

#endif
