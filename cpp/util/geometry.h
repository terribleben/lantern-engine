
// geometry.h

#ifndef __LANTERN_GEOMETRY_H__
#define __LANTERN_GEOMETRY_H__

#include <cstdlib>

#include "Point3f.h"

Point3f crossProduct(Point3f a, Point3f b);

// calculate whether the triangle (a, b, c) contains the Point3f p.
bool triangleContains(Point3f a, Point3f b, Point3f c, Point3f p);

// calculate whether the rectangle (topLeft, topRight, bottomLeft, bottomRight) contains the Point3f p.
bool rectangleContains(Point3f topLeft, Point3f topRight, Point3f bottomLeft, Point3f bottomRight, Point3f p);

// same as above, but only works for rectangles that are oriented with the xy axes of the display.
bool simpleRectangleContains(Point3f topLeft, Point3f bottomRight, Point3f p);

// calculate whether the Point3f p lies in the plane of the triangle (a, b, c).
bool trianglePlaneContains(Point3f a, Point3f b, Point3f c, Point3f p);

// decide whether n is between a and b, inclusive.
bool between(float n, float a, float b);

// angle between two points
float angleTo(Point3f &p1, Point3f &p2);

// difference between two angles
float diffAngle(float angle1, float angle2);

#endif