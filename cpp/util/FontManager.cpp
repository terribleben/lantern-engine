

#include "FontManager.h"

FontManager& FontManager::getInstance() {
	static FontManager theMgr;
	return theMgr;
}

FontManager::FontManager() {
    hasSetWidths = false;
    
    // TODO let this be a config file of some kind
    /* Font* defaultFont = new Font();
       defaultFont->loadTexture("my_font_texture_name", 512, 512, 32, 32, 0, 0);
       setFont("default", defaultFont); */
    
    for (map<string, Font*>::iterator itr = fonts.begin(); itr != fonts.end(); itr++) {
        (*itr).second->setCharacterWidths(getCharacterWidths((*itr).first), 0, 256);
    }
}

void FontManager::setFont(string name, Font* font) {
    if (fonts.find(name) != fonts.end()) {
        delete fonts[name];
    }
    fonts[name] = font;
}

Font* FontManager::getFont(string name) {
    if (fonts.find(name) != fonts.end())
        return fonts[name];
    return NULL;
}

float* FontManager::getCharacterWidths(string fontName) {
    // hard-coded kerning values.
    // TODO let this be an external resource.
    if (!hasSetWidths) {
        for (unsigned int ii = 0; ii < 255; ii++) {
            widths[ii] = 1.0f;
        }
        hasSetWidths = true;
    }
    
    /* if (whichFont == MY_OTHER_FONT) {
        if (!hasSetWidths) {
            for (unsigned int i = 0; i < 255; i++) {
                widths[i] = 1.0f;
            }
            widths['I'] = widths['!'] = widths[':'] = widths[';'] = widths['\''] = widths['"'] = widths['.'] = widths[','] = 0.5f;
            widths['%'] = widths['O'] = widths['D'] = widths['N'] = widths['R'] = widths['X'] = 1.1f;
            widths['M'] = widths['W'] = 1.2f;
            widths['P'] = widths['T'] = widths['L'] = 0.9f;
            widths[' '] = 0.7f;
        }
    } */
    
    return widths;
}

FontManager::~FontManager() {
    for (map<string, Font*>::iterator itr = fonts.begin(); itr != fonts.end(); itr++) {
        delete (*itr).second;
    }
}