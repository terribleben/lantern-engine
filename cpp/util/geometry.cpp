
// geometry.cpp

#include "geometry.h"

Point3f nearestPointOnSegmentToPoint(Point3f a, Point3f b, Point3f p) {
    Point3f position;
    
    if (b.x == a.x) {
        position.x = b.x;
        position.y = p.y;
    } else if (b.y == a.y) {
        position.x = p.x;
        position.y = b.y;
    } else {
        float slope = getSlope(a, b);
        float slopeInv = 1.0f / slope;
        position.x = (slope * a.x - a.y + p.y + slopeInv * p.x) / (slope + slopeInv);
        position.y = slope * (position.x - a.x) + a.y;
    }
    return position;
}

Point3f crossProduct(Point3f a, Point3f b) {
	Point3f product(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	return product;
}

// are the points p1 and p2 on the same side of the line containing points a and b?
bool sameSideOfLine(Point3f p1, Point3f p2, Point3f a, Point3f b) {
	Point3f cross1 = crossProduct(b - a, p1 - a);
	Point3f cross2 = crossProduct(b - a, p2 - a);
	return ((cross1.x * cross2.x + cross1.y * cross2.y + cross1.z * cross2.z) >= 0);
}

bool triangleContains(Point3f a, Point3f b, Point3f c, Point3f p) {
	return !(
			   !sameSideOfLine(p, a, b, c)
			|| !sameSideOfLine(p, b, a, c)
			|| !sameSideOfLine(p, c, a, b)
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

float getSlope(Point3f &p1, Point3f &p2) {
	if (p1.x == p2.x) return INFINITY;
	return (p1.y - p2.y) / (p1.x - p2.x);
}

bool between(float n, float a, float b) {
	if (a < b)
		return (n >= a && n <= b);
	else
		return (n >= b && n <= a);
}

// decides whether the line segments (p1, p2) and (p3, p4) intersect.
// note that this is only designed to operate in two dimensions, so the z coordinate
// of these points is ignored.
bool segmentsIntersect(Point3f &p1, Point3f &p2, Point3f &p3, Point3f &p4, Point3f* intersectionPosition) {
	float m1 = getSlope(p1, p2);
	float m2 = getSlope(p3, p4);
    
    if (m1 == m2) return false;
	
	float intX, intY;
	
	if (m1 == INFINITY) {
		intX = p1.x;
		intY = m2 * (intX - p3.x) + p3.y;
	} else if (m2 == INFINITY) {
		intX = p3.x;
		intY = m1 * (intX - p1.x) + p1.y;
	} else {
		intX = (-p1.y + (m1 * p1.x) + p3.y - (m2 * p3.x)) / (m1 - m2);
		intY = m1 * (intX - p1.x) + p1.y;
	}
	
	if (
			between(intX, p1.x, p2.x)
			&& between(intX, p3.x, p4.x)
			&& between(intY, p1.y, p2.y)
			&& between(intY, p3.y, p4.y)
		) {
		
		if (intersectionPosition != NULL) {
			intersectionPosition->x = intX;
			intersectionPosition->y = intY;
		}
		return true;
	}
	return false;
}

float angleTo(Point3f &p1, Point3f &p2) {
	return atan2(p2.y - p1.y, p2.x - p1.x);
}

float diffAngle(float angle1, float angle2) {
	while (angle1 > M_2_PI) angle1 -= M_2_PI;
	while (angle2 > M_2_PI) angle2 -= M_2_PI;
	while (angle1 < 0) angle1 += M_2_PI;
	while (angle2 < 0) angle2 += M_2_PI;
	
	float diff = angle1 - angle2;
	if (fabs(diff) <= M_PI) return diff;
	
	while (angle1 > M_PI) angle1 -= M_2_PI;
	while (angle2 > M_PI) angle2 -= M_2_PI;
	while (angle1 < -M_PI) angle1 += M_2_PI;
	while (angle2 < -M_PI) angle2 += M_2_PI;

	return angle1 - angle2;
}