
// geometry.cpp

#include "geometry.h"
#include "Line3f.h"
#include "Triangle3f.h"

Point3f crossProduct(Point3f a, Point3f b) {
	Point3f product(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	return product;
}

bool rectangleContains(Point3f topLeft, Point3f topRight, Point3f bottomLeft, Point3f bottomRight, Point3f p) {
	return (Triangle3f(topLeft, topRight, bottomLeft).contains(p) || Triangle3f(bottomLeft, topRight, bottomRight).contains(p));
}

bool simpleRectangleContains(Point3f topLeft, Point3f bottomRight, Point3f p) {
	Point3f _topRight(bottomRight.x, topLeft.y, 0);
	Point3f _bottomLeft(topLeft.x, bottomRight.y, 0);
	return rectangleContains(topLeft, _topRight, _bottomLeft, bottomRight, p);
}

bool between(float n, float a, float b) {
	if (a < b)
		return (n >= a && n <= b);
	else
		return (n >= b && n <= a);
}
