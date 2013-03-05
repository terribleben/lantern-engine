//
//  Rectangle3f.h
//  lantern
//

#ifndef __LANTERN_RECTANGLE_H__
#define __LANTERN_RECTANGLE_H__

#include "Point3f.h"

class Rectangle3f {
public:
    // origin: center
    Point3f origin, size;
    
    // default zero; if nonzero, this class uses some extra memory to cache the four oriented corners.
    void setOrientation(float orientation);
    float getOrientation();
    
public:
    Rectangle3f() : orientation(0), corners(0x0) {  }
    Rectangle3f(Point3f origin, Point3f size, float orientation);
    Rectangle3f(Point3f bottomLeft, Point3f topRight); // no validity check here!
    
    Rectangle3f(const Rectangle3f& other) { *this = other; }
    ~Rectangle3f();
    
    bool contains(Point3f& p);
    
public:
    const Rectangle3f& operator =(const Rectangle3f&);
    
protected:
    float orientation;
    Point3f* corners;
};

#endif
