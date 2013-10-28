//
// FontManager
// A singleton instance responsible for loading fonts and keeping a reference to them.
//
// addFontFromFile() takes a text file, where the contents of the file is a serialized Font object (see Font::serialize()).
// There is a sample font bundled in white_bevel.lanternfont and white_bevel.png, which is mapped to the key "default".
//

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
    
    void addFont(Font* font, string key);
    bool addFontFromFile(string filename, string key);
    void saveFontToFile(Font* font, string filename);
    
    Font* getFont(string key);
    
private:
	FontManager();
	FontManager(const FontManager& other);
	~FontManager();
    
    map<string, Font*> fonts;
};

#endif