
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "Font.h"
#include "Rectangle3f.h"
#include "ResourceManager.h"

#define LANTERN_FONT_VERSION 1.0f

Font::Font() {
    charWidth = charHeight = 0;
    texWidth = texHeight = 0;
    tex = 0;
    idxZero = 0;
    loaded = false;
    characterWidths = NULL;
}

Font::~Font() {
    if (loaded) {
        glDeleteTextures(1, &tex);
        tex = 0;
    }
    if (characterWidths) {
        free(characterWidths);
        characterWidths = NULL;
    }
}

/**
 * format:
 * version
 * tex name
 * tex width
 * tex height
 * char width
 * char height
 * chars per row
 * zero index
 * is fixed width?
 * (optional) character widths
 */
string Font::serialize() {
    std::ostringstream stream(std::ostringstream::out);
    if (!stream.good()) {
        return "";
    }
    
    stream << LANTERN_FONT_VERSION << " ";
    stream << textureFilename << " ";
    stream << texWidth << " " << texHeight << " " << charWidth << " " << charHeight << " ";
    stream << charsPerRow << " " << idxZero << " ";
    stream << (characterWidths == NULL) << " ";
    
    if (characterWidths) {
        for (int ii = 0; ii < 256; ii++) {
            stream << characterWidths[ii] << " ";
        }
    }
    
    return stream.str();
}

bool Font::loadFromString(string serializedFont) {
    if (loaded)
        return false;
    
    std::istringstream stream(serializedFont, std::istringstream::in);
    if (!stream.good())
        return false;
    
    float fontVersion;
    stream >> fontVersion;
    if (fontVersion > LANTERN_FONT_VERSION) // assume future versions suck
        return false;
    
    stream >> textureFilename;
    stream >> texWidth >> texHeight >> charWidth >> charHeight;
    stream >> charsPerRow >> idxZero;
    
    bool isFixedWidth;
    stream >> isFixedWidth;
    if (isFixedWidth)
        characterWidths = NULL;
    else {
        float widths[256];
        for (int ii = 0; ii < 256; ii++)
            stream >> widths[ii];
        setCharacterWidths(widths, 0, 256);
    }
    
    return loadTexture();
}

bool Font::loadTexture(string texName, unsigned int texWidth, unsigned int texHeight, unsigned int charWidth, unsigned int charHeight, unsigned int charsPerRow, int idxZero) {
    if (loaded)
        return false;
    
    characterWidths = NULL;
    this->textureFilename = texName;
	this->idxZero = idxZero;
	this->charWidth = charWidth;
	this->charHeight = charHeight;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
    
    if (charsPerRow == 0)
        this->charsPerRow = (texWidth / charWidth);
    else
        this->charsPerRow = charsPerRow;

    return loadTexture();
}

bool Font::loadTexture() {
    if (loaded)
        return false;
    
    glGenTextures(1, &tex);
	
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    ResourceManager::getInstance().loadTexture(textureFilename.c_str(), "png");
	
	if (tex == 0) {
        fprintf(stdout, "Font: Error loading font %s\n", textureFilename.c_str());
		loaded = false;
        return false;
	}
    
    loaded = true;
    return true;
}

void Font::setCharacterWidths(float* widths, int idxStart, int length) {
    if (characterWidths) {
        free(characterWidths);
    }
    if (widths && idxStart >= 0) {
        characterWidths = (float*) malloc(sizeof(float) * 256);
        
        for (int ii = 0; ii < 256; ii++) {
            if (ii >= idxStart && ii < idxStart + length)
                characterWidths[ii] = widths[ii - idxStart];
            else
                characterWidths[ii] = 1.0f;
        }
    } else
        characterWidths = NULL;
}


void Font::drawCharacter(float x, float y, char c) {
  this->drawCharacterExt(x, y, c, 1.0);
}

void Font::drawString(float x, float y, string s) {
  this->drawStringExt(x, y, s, LANTERN_FONT_ALIGN_LEFT, LANTERN_FONT_ALIGN_TOP, 1, 1);
}

void Font::drawCharacterExt(float x, float y, char c, float scale) {
	if (!loaded) return;
	if (c == ' ') return;
	
	int charIdx = (c == 0) ? ('z' + 11 + idxZero) : c + idxZero;
    if (charIdx < 0) return;
	
	unsigned int xI = charIdx % charsPerRow;
	unsigned int yI = charIdx / charsPerRow;
	unsigned int xF = xI + 1;
	unsigned int yF = yI + 1;

	glBindTexture(GL_TEXTURE_2D, tex);

	float scaledCharWidth = charWidth * scale;
	float scaledCharHeight = charHeight * scale;
	
	GLfloat texCoords[] = {
        (float)(xI * charWidth) / (float)texWidth, ((float)(yI * charHeight) / (float)texHeight) - (1.0 / texHeight),
        (float)(xF * charWidth) / (float)texWidth, ((float)(yI * charHeight) / (float)texHeight) - (1.0 / texHeight),
        (float)(xI * charWidth) / (float)texWidth, ((float)(yF * charHeight) / (float)texHeight),
        (float)(xF * charWidth) / (float)texWidth, ((float)(yF * charHeight) / (float)texHeight),
	};
	
	GLfloat vertices[] = {
		x, y, 0,
		x + scaledCharWidth, y, 0,
		x, y - scaledCharHeight, 0,
		x + scaledCharWidth, y - scaledCharHeight, 0
	};
	
    drawTexture(vertices, texCoords);
}

float Font::getStringWidth(string s, float scale, float spacing) {
    float oneCharWidth = charWidth * scale * spacing;
    
    if (characterWidths == NULL)
        return (oneCharWidth * (s.length() - 1)) + (charWidth * scale);
    else if (s.length() < 1)
        return 0;
    else {
        float width = 0;
        for (unsigned int i = 0, n = s.length() - 1; i < n; i++) {
            width += oneCharWidth * characterWidths[s[i]];
        }
        width += charWidth * scale * characterWidths[s[s.length() - 1]];
        return width;
    }
}

unsigned int Font::drawStringExtWrap(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing, float maxWidth) {
    return drawStringExtWrap(x, y, s, hAlign, vAlign, scale, spacing, maxWidth, 0);
}

unsigned int Font::drawStringExtWrap(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing, float maxWidth, float maxHeight) {
	if (!loaded) return 0;
	
	int prevSpaceIdx = 0, prevLineIdx = 0;
	float width = 0, height = 0;
	int numLines = 0;
	float charWidthSpaced = charWidth * scale * spacing;
	float charHeightSpaced = charHeight * scale;
	
	for (unsigned int i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '\n') {
            width = maxWidth;
        } else {
            if (characterWidths != NULL)
                width += charWidthSpaced * characterWidths[c];
            else
                width += charWidthSpaced;
            
            if (c == ' ' || c == '\t') prevSpaceIdx = i;
        }
		
		if (i == s.length() - 1) {
			drawStringExt(x, y - charHeightSpaced * numLines, s.substr(prevLineIdx, s.length() - prevLineIdx), hAlign, vAlign, scale, spacing);
		}	
		else if (width >= maxWidth) {
            if (maxHeight > 0 && height + charHeightSpaced * 2.0f > maxHeight) {
                if (height + charHeightSpaced < maxHeight)
                    drawStringExt(x, y - height, s.substr(prevLineIdx), hAlign, vAlign, scale, spacing, maxWidth);
                break;
            } else {
                if (prevSpaceIdx > prevLineIdx)
                    drawStringExt(x, y - height, s.substr(prevLineIdx, prevSpaceIdx - prevLineIdx), hAlign, vAlign, scale, spacing);
                prevLineIdx = prevSpaceIdx + 1;
                numLines++;
                height += charHeightSpaced;
                width = getStringWidth(s.substr(prevSpaceIdx + 1, i - prevSpaceIdx + 1), scale, spacing);//charWidthSpaced * (1 + i - prevSpaceIdx);
            }
		}
	}
	
	return numLines + 1;
}

unsigned int Font::getNumWrapLines(string s, float scale, float spacing, float maxWidth) {
	if (!loaded) return 0;
	
	int prevSpaceIdx = 0, prevLineIdx = 0;
	float width = 0;
	int numLines = 0;
	float charWidthSpaced = charWidth * scale * spacing;
	
	for (unsigned int i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '\n') {
            width = maxWidth;
        } else {
            if (characterWidths == NULL)
                width += charWidthSpaced;
            else
                width += charWidthSpaced * characterWidths[c];
            
            if (c == ' ' || c == '\t') prevSpaceIdx = i;
        }
		
		if (width >= maxWidth) {
			prevLineIdx = prevSpaceIdx + 1;
			numLines++;
			width = getStringWidth(s.substr(prevSpaceIdx + 1, i - prevSpaceIdx + 1), scale, spacing);//charWidthSpaced * (1 + i - prevSpaceIdx);
		}
	}
	
	return numLines + 1;
}

void Font::drawStringExt(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing) {
    drawStringExt(x, y, s, hAlign, vAlign, scale, spacing, 0);
}

void Font::drawStringExt(float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing, float maxWidth) {
	if (!loaded) return;
	
	float xAlign = x, yAlign = y;
	float charWidthSpaced = charWidth * scale * spacing;
    float width = 0;
	
	float stringWidth = getStringWidth(s, scale, spacing);
    if (maxWidth > 0 && stringWidth > maxWidth)
        stringWidth = maxWidth;
    
	float stringHeight = charHeight * scale;
	
	if (hAlign == LANTERN_FONT_ALIGN_CENTER) xAlign -= stringWidth / 2.0f;
	if (hAlign == LANTERN_FONT_ALIGN_RIGHT) xAlign -= stringWidth;
	if (vAlign == LANTERN_FONT_ALIGN_MIDDLE) yAlign += stringHeight / 2.0f;
	if (vAlign == LANTERN_FONT_ALIGN_BOTTOM) yAlign += stringHeight;
	
	for (unsigned int i = 0; i < s.length(); i++) {
        float charWidth = (characterWidths == NULL) ? 1.0f : characterWidths[s[i]];
        if (maxWidth > 0 && width + charWidthSpaced * 2 >= maxWidth) {
            this->drawCharacterExt(xAlign + width - (charWidthSpaced * (1.0f - charWidth) * 0.5f), yAlign, 0, scale);
            break;
        } else
            this->drawCharacterExt(xAlign + width - (charWidthSpaced * (1.0f - charWidth) * 0.5f), yAlign, s[i], scale);
        width += charWidthSpaced * charWidth;
	}
}

bool Font::stringContainsExt(Point3f p, float margin, float x, float y, string s, unsigned int hAlign, unsigned int vAlign, float scale, float spacing) {
	if (!loaded) return false;
	
	float xAlign = x, yAlign = y;
	
	float stringWidth = getStringWidth(s, scale, spacing);
	float stringHeight = charHeight * scale;
	
	if (hAlign == LANTERN_FONT_ALIGN_CENTER) xAlign -= stringWidth / 2.0f;
	if (hAlign == LANTERN_FONT_ALIGN_RIGHT) xAlign -= stringWidth;
	if (vAlign == LANTERN_FONT_ALIGN_MIDDLE) yAlign += stringHeight / 2.0f;
	if (vAlign == LANTERN_FONT_ALIGN_BOTTOM) yAlign += stringHeight;
    
    return Rectangle3f(Point3f(xAlign - margin, yAlign - stringHeight - margin, 0), Point3f(xAlign + stringWidth + margin, yAlign + margin, 0)).contains(p);
}

