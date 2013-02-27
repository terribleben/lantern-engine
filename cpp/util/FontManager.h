

#ifndef __LANTERN_FONT_MGR_H__
#define __LANTERN_FONT_MGR_H__

#include "Font.h"

#include <map>
#include <string>

using std::string;
using std::map;

class FontManager {
public:
    static FontManager& getInstance();
    
    void setFont(string name, Font* font);
    Font* getFont(string name);
    
private:
    float* getCharacterWidths(string fontName);
    float widths[255];
    bool hasSetWidths;
    
	FontManager();
	FontManager(const FontManager& other);
	~FontManager();
    
    map<string, Font*> fonts;
};

#endif