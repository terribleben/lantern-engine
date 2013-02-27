//
//  Font
//  lantern
//
//  A simple class for drawing fixed-size texture fonts.
//  Assumes we are operating in an orthographic projection.
//

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
    
    /**
     * load a texture into this font object. Must be called before using any of the draw methods.
     * params: dimensions of the whole texture image, dimensions of one character, and the index
     *         (on the image) at which the NULL character (ascii zero) appears.
     */
	void loadTexture(string texName, unsigned int texWidth, unsigned int texHeight, unsigned int charWidth, unsigned int charHeight, unsigned int charsPerRow, int idxZero);
    
    // Use this to add some notion of kerning to a font. Each width is a scale > 0 <= 1.
    // Can pass NULL to indicate a font is fixed-width (i.e. all values are 1).
    void setCharacterWidths(float* widths, int idxStart, int length);
    
public:
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
    
private:
	int idxZero;
	unsigned int charWidth, charHeight, texWidth, texHeight;
    unsigned int charsPerRow, charsPerCol;
	GLuint tex;
	bool loaded;
    
    float* characterWidths;
};

#endif
