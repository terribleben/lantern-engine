

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
    
    void addFont(Font* font, string name);
    void addFontFromFile(string filename, string name);
    void saveFontToFile(Font* font, string filename);
    
    Font* getFont(string name);
    
private:
	FontManager();
	FontManager(const FontManager& other);
	~FontManager();
    
    map<string, Font*> fonts;
};

#endif