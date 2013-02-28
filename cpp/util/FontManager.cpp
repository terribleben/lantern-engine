

#include "FontManager.h"
#include "ResourceManager.h"

FontManager& FontManager::getInstance() {
	static FontManager theMgr;
	return theMgr;
}

FontManager::FontManager() {

}

void FontManager::addFont(Font* font, string name) {
    if (fonts.find(name) != fonts.end()) {
        delete fonts[name];
    }
    fonts[name] = font;
}

void FontManager::addFontFromFile(string filename, string name) {
    const char* path = ResourceManager::getInstance().prependResourcePath(filename.c_str());
    string serializedFont = ResourceManager::getInstance().fileContentsAsString(path);
    
    if (serializedFont.length()) {
        Font* f = new Font();
        f->loadFromString(serializedFont);
        this->addFont(f, name);
    }
}

void FontManager::saveFontToFile(Font* font, string filename) {
    const char* path = ResourceManager::getInstance().prependResourcePath(filename.c_str());
    string serializedFont = font->serialize();
    ResourceManager::getInstance().writeStringToFile(serializedFont, path);
}

Font* FontManager::getFont(string name) {
    if (fonts.find(name) != fonts.end())
        return fonts[name];
    return NULL;
}

FontManager::~FontManager() {
    for (map<string, Font*>::iterator itr = fonts.begin(); itr != fonts.end(); itr++) {
        delete (*itr).second;
    }
}