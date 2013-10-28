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
    if (!FontManager::getInstance().addFontFromFile("white_bevel.lanternfont", "default")) {
        fprintf(stdout, "Warning: Failed to load example font from file");
    }
    
    // load and queue the first view
    addView(new ViewPlay(), "play");
    setInitialView("play");
}
