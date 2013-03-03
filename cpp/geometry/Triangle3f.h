//
//  Triangle3f.h
//  lantern
//

#ifndef __LANTERN_TRIANGLE_H__
#define __LANTERN_TRIANGLE_H__

#include "Point3f.h"

class Triangle3f {
public:
    Point3f a, b, c;
    
public:
    Triangle3f() {  }
    Triangle3f(Point3f a, Point3f b, Point3f c) { this->a = a; this->b = b; this->c = c; }
    Triangle3f(const Triangle3f& other) { *this = other; }

    bool contains(Point3f& p);

    // true if (a, b, c) and (p) are coplanar. (obviously this method is only interesting in 3D)
    bool planeContains(Point3f& p);

public:
    const Triangle3f& operator =(const Triangle3f&);
};

#endif
