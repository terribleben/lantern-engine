//
//  LanternExample.cpp
//  lantern-example
//

#include "LanternExample.h"
#include "ViewPlay.h"
#include "FontManager.h"

void LanternExample::gameWillBegin() {
    Lantern::gameWillBegin();
    
    // load the example font
    FontManager::getInstance().addFontFromFile("white_bevel.lanternfont", "default");
    
    // load and queue the first view
    addView(new ViewPlay(), "play");
    setInitialView("play");
}
