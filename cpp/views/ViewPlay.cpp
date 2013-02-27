//
//  ViewPlay.cpp
//  lantern
//

#include <iostream>
#include "draw.h"
#include "ViewPlay.h"

void ViewPlay::init(ViewParams* p) {
    
}

void ViewPlay::step() {
    
}

void ViewPlay::draw() {
    glPushMatrix();
    
    // test rectangle
    glColor4f(1, 0, 0, 1);
    drawRectangle(Point3f(100, 100, 0), Point3f(200, 200, 0), true);
}

void ViewPlay::drawForeground() {
    glPopMatrix();
}

void ViewPlay::event(Event& e) {
    
}

bool ViewPlay::isFinished() {
    return false;
}

void ViewPlay::stop(ViewParams* p) {
    
}