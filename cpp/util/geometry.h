
// geometry.h

#ifndef __LANTERN_GEOMETRY_H__
#define __LANTERN_GEOMETRY_H__

#include <cstdlib>

#include "Point3f.h"

// calculate whether the rectangle (topLeft, topRight, bottomLeft, bottomRight) contains the Point3f p.
bool rectangleContains(Point3f topLeft, Point3f topRight, Point3f bottomLeft, Point3f bottomRight, Point3f p);

// same as above, but only works for rectangles that are oriented with the xy axes of the display.
bool simpleRectangleContains(Point3f topLeft, Point3f bottomRight, Point3f p);

// decide whether n is between a and b, inclusive.
bool between(float n, float a, float b);

#endif