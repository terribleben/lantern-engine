
// draw.h

#ifndef __LANTERN_DRAW_H__
#define __LANTERN_DRAW_H__

#include <OpenGLES/ES1/gl.h>
#include "Point3f.h"

void lookAt(double eye_x, double eye_y, double eye_z, double at_x, double at_y, double at_z, double up_x, double up_y, double up_z);

void drawRectangle(Point3f topLeft, Point3f bottomRight, bool filled);
void drawQuadrilateral(Point3f p1, Point3f p2, Point3f p3, Point3f p4, bool filled);
void drawTriangle(Point3f p1, Point3f p2, Point3f p3, bool filled);
void drawLine(Point3f p1, Point3f p2);
void drawLineWithColor(Point3f p1, Point3f p2, Point3f col1, Point3f col2, float alpha1, float alpha2);

void drawPolygon(Point3f* vertices, unsigned int numVertices, bool filled);

// simplest texture drawing method.
void drawTexture(Point3f position, float width, float height);

// slightly more explicit texture drawing methods.
// corners in a z shape, top to bottom
void drawTexture(Point3f p1, Point3f p2, Point3f p3, Point3f p4);
void drawTexture(Point3f p1, Point3f p2, Point3f p3, Point3f p4, bool xReversed, bool yReversed);
void drawTexture(GLfloat* vertices, GLfloat* texCoords);

void transformVertices(GLfloat* vertices, unsigned int numVertices, Point3f &position, float orientation);

void drawQuadrilateral(Point3f p1, Point3f p2, Point3f p3, Point3f p4, bool filled, Point3f &position, float orientation);
void drawTriangle(Point3f p1, Point3f p2, Point3f p3, bool filled, Point3f &position, float orientation);

// hex color macro, e.g. red is glColor4fFromHex(0xff0000, 1.0f)
#define glColor4fFromHex(rgbValue,a) glColor4f(((float)((rgbValue & 0xFF0000) >> 16))/255.0, ((float)((rgbValue & 0xFF00) >> 8))/255.0, ((float)(rgbValue & 0xFF))/255.0, a)

#endif