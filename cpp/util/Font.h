/**
 * simple class for drawing fixed-width, fixed-height texture fonts.
 */

#ifndef __LANTERN_FONT_H__
#define __LANTERN_FONT_H__

#include "draw.h"
#include <string>
using std::string;

#define LANTERN_FONT_ALIGN_LEFT 0
#define LANTERN_FONT_ALIGN_CENTER 1
#define LANTERN_FONT_ALIGN_RIGHT 2

#define LANTERN_FONT_ALIGN_TOP 3
#define LANTERN_FONT_ALIGN_MIDDLE 4
#define LANTERN_FONT_ALIGN_BOTTOM 5

class Font {
public:
	Font();
	~Font();
	void loadTexture(string texName, unsigned int texWidth, unsigned int texHeight, unsigned int charWidth, unsigned int charHeight, unsigned int charsPerRow, int idxZero);
    void setCharacterWidths(float* widths);
    
	void drawCharacter(float x, float y, char c);
	void drawCharacterExt(float x, float y, char c, float scale);
	void drawString(float x, float y, string s);
    
    void drawStringExt(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing);
	void drawStringExt(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing, float maxWidth);
    
	unsigned int drawStringExtWrap(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing, float maxWidth);
	unsigned int drawStringExtWrap(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing, float maxWidth, float maxHeight);
	unsigned int getNumWrapLines(string s, float scale, float spacing, float maxWidth);
    
    float getStringWidth(string s, float scale, float spacing);
	bool stringContainsExt(Point3f p, float margin, float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing);
	
    bool isAdditive;
    
private:
	int idxZero;
	unsigned int charWidth, charHeight, texWidth, texHeight;
    unsigned int charsPerRow, charsPerCol;
	GLuint tex;
	bool loaded;
    
    float* characterWidths;
};

#endif
