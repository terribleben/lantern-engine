//
//  lantern.cpp
//  lantern
//

#include <iostream>

#include "lantern.h"
#include "view.h"
#include "ParticleManager.h"


Lantern& Lantern::getInstance() {
    static Lantern theLantern;
    return theLantern;
}

void Lantern::init() {
    glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_VERTEX_ARRAY);
    
    screenWidth = screenHeight = 0;
    view = new ViewPlay();
    ViewParams p;
    view->init(&p);
}

void Lantern::setDimensions(GLfloat width, GLfloat height) {
    screenWidth = width;
    screenHeight = height;
}

void Lantern::setScale(GLfloat scale) {
    screenScale = scale;
}

void Lantern::setOrientation(bool isPortrait) {
    this->isPortrait = isPortrait;
}

void Lantern::stop() {
    if (view != NULL) {
        delete view;
        view = NULL;
    }
}

void Lantern::transitionView() {
    if (view != NULL && view->isFinished()) {
        ViewParams p;
        view->stop(&p);
        delete view;
        
        switch (p.nextView) {
            case LANTERN_VIEW_PLAY:
                view = new ViewPlay();
                break;
        }
        
        view->init(&p);
    }
}

void Lantern::event(Event& e) {
    if (view) {
        if (isPortrait) {
            if (e.type == LANTERN_EVENT_TOUCH_DOWN || e.type == LANTERN_EVENT_TOUCH_UP) {
                float* touchPoint = (float*)e.data;
                
                // invert x, then swap y and x
                touchPoint[0] = (screenWidth / screenScale) - touchPoint[0]; // for relative, just touchPoint[0] = -touchPoint[0]
                
                float swap = touchPoint[1];
                touchPoint[1] = touchPoint[0];
                touchPoint[0] = swap;
            }
        }
        
        view->event(e);
    }
}

void Lantern::draw() {
    glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                  
    glOrthof(0, screenWidth / screenScale, 0, screenHeight / screenScale, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (isPortrait) {
        // translate
        glTranslatef((screenWidth / screenScale) / 2, (screenHeight / screenScale) / 2, 0);
        // rotate around z
        glRotatef(90, 0, 0, 1);
        // translate
        glTranslatef(-(screenHeight / screenScale) / 2, -(screenWidth / screenScale) / 2, 0);
    }
    
    if (view != NULL)
        view->step();
    ParticleManager::getInstance().step();
    
    transitionView();
    
    if (view != NULL) {
        view->draw();
        ParticleManager::getInstance().draw();
        view->drawForeground();
    }
}