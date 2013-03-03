
// geometry.cpp

#include "geometry.h"
#include "Line3f.h"

Point3f crossProduct(Point3f a, Point3f b) {
	Point3f product(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	return product;
}

bool triangleContains(Point3f a, Point3f b, Point3f c, Point3f p) {
    return !(
                !Line3f(p, a).sameSide(b, c)
             || !Line3f(p, b).sameSide(a, c)
             || !Line3f(p, c).sameSide(a, b)
             );
}

bool rectangleContains(Point3f topLeft, Point3f topRight, Point3f bottomLeft, Point3f bottomRight, Point3f p) {
	return (triangleContains(topLeft, topRight, bottomLeft, p) || triangleContains(bottomLeft, topRight, bottomRight, p));
}

bool simpleRectangleContains(Point3f topLeft, Point3f bottomRight, Point3f p) {
	Point3f _topRight(bottomRight.x, topLeft.y, 0);
	Point3f _bottomLeft(topLeft.x, bottomRight.y, 0);
	return rectangleContains(topLeft, _topRight, _bottomLeft, bottomRight, p);
}

bool trianglePlaneContains(Point3f a, Point3f b, Point3f c, Point3f p) {	
	// we decide whether the vectors pa, pb, and pc are coplanar.
	// this is true if the determinant of this matrix is zero:
	// [[ pX - aX  pY - aY  pZ - aZ]
	//  [ pX - bX  pY - bY  pZ - bZ]
	//  [ pX - cX  pY - cY  pZ - cZ]]
	
	Point3f diffA = p - a;
	Point3f diffB = p - b;
	Point3f diffC = p - c;
	
	float det =   diffA.x * ((diffB.y * diffC.z) - (diffB.z * diffC.y))
				- diffA.y * ((diffB.x * diffC.z) - (diffB.z * diffC.x))
				+ diffA.z * ((diffB.x * diffC.y) - (diffB.y * diffC.x));

	return (det == 0);
}

bool between(float n, float a, float b) {
	if (a < b)
		return (n >= a && n <= b);
	else
		return (n >= b && n <= a);
}
