//
//  ViewPlay.cpp
//  lantern
//

#include <iostream>
#include "draw.h"
#include "ViewPlay.h"
#include "FontManager.h"

void ViewPlay::init(ViewParams* p) {
    View::init(p);
}

void ViewPlay::step() {
    
}

void ViewPlay::draw() {
    glPushMatrix();
    
    // test message
    glColor4f(1, 1, 1, 1);
    FontManager::getInstance().getFont("default")->drawStringExt(10, 10, "LANTERN", LANTERN_FONT_ALIGN_LEFT, LANTERN_FONT_ALIGN_BOTTOM, 0.7f, 0.7f);
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