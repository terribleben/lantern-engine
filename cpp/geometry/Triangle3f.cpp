//
//  Triangle3f.cpp
//  lantern
//

#include "Triangle3f.h"
#include "Line3f.h"

Point3f& Triangle3f::operator [] (unsigned int index) {
    return *(Point3f*)(&a + sizeof(Point3f) * index);
}

const Point3f& Triangle3f::operator [] (unsigned int index) const {
    return *(Point3f*)(&a + sizeof(Point3f) * index);
}

const Triangle3f& Triangle3f::operator =(const Triangle3f& other) {
    a = other.a;
    b = other.b;
    c = other.c;
    return *this;
}

bool Triangle3f::contains(Point3f& p) {
    return !(
             !Line3f(p, a).sameSide(b, c)
             || !Line3f(p, b).sameSide(a, c)
             || !Line3f(p, c).sameSide(a, b)
             );
}

bool Triangle3f::planeContains(Point3f& p) {
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
