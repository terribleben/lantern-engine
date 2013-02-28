//
//  lantern.cpp
//  lantern
//

#include <iostream>

#include "lantern.h"
#include "view.h"
#include "ParticleManager.h"
#include "FontManager.h"


Lantern& Lantern::getInstance() {
    static Lantern theLantern;
    return theLantern;
}

void Lantern::init() {
    if (!isRunning) {
        isRunning = true;
        
        // reset graphics properties
        screenWidth = screenHeight = 0;
        screenScale = 1.0f;
        isPortrait = false;
        
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        // load the example font
        FontManager::getInstance().addFontFromFile("white_bevel.lanternfont", "default");
        
        // start the first view
        addView(new ViewPlay(), "play");
        transitionView();
    }
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
    if (isRunning) {
        map<string, View*>::iterator viewItr = views.begin();
        while (viewItr != views.end()) {
            delete (*viewItr).second;
            views.erase(viewItr++);
        }
        view = NULL;
        isRunning = false;
    }
}

void Lantern::addView(View* view, string key) {
    if (views.find(key) != views.end()) {
        delete views[key];
    }
    views[key] = view;
}

void Lantern::transitionView() {
    ViewParams p;
    bool shouldTransition = false;
    
    if (view == NULL) {
        p.nextView = views.begin()->first;
        shouldTransition = true;
    }
    else if (view->isFinished()) {
        shouldTransition = true;
        view->stop(&p);
    }
    
    if (shouldTransition) {
        if (p.nextView.length() && views.find(p.nextView) != views.end()) {
            view = views[p.nextView];
            view->init(&p);
        } else {
            // some kind of problem assigning next view; crash
            fprintf(stdout, "Lantern: Invalid next view when transitioning between views.");
            int* crash = 0;
            *crash = 0;
        }
    }
}

void Lantern::event(Event& e) {
    if (view) {
        if (isPortrait) {
            switch (e.type) {
                case LANTERN_EVENT_TOUCH_DOWN: case LANTERN_EVENT_TOUCH_UP: case LANTERN_EVENT_TOUCH_MOVE: {
                    float* touchPoint = (float*)e.data;
                    
                    // invert x, then swap y and x
                    touchPoint[0] = (screenWidth / screenScale) - touchPoint[0]; // for relative, just touchPoint[0] = -touchPoint[0]
                    
                    float swap = touchPoint[1];
                    touchPoint[1] = touchPoint[0];
                    touchPoint[0] = swap;
                    break;
                }
                default: {
                    break;
                }
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