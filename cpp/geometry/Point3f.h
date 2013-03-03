//
//  Point.h
//  lantern
//

#ifndef __LANTERN_POINT_H__
#define __LANTERN_POINT_H__

#include <math.h>

class Point3f {
public:
    float x, y, z;
    
public:
    // constructors
    Point3f() : x(0), y(0), z(0) { }
    Point3f(float, float, float);
    Point3f(const Point3f&);
    ~Point3f() { }
    
    // setters
    void set(float, float, float);
    void setAll(float);
    
    // accessors
    float& operator [] (unsigned int index);
    const float& operator [] (unsigned int index) const;
    
    // manipulators
    void normalize();
    
    const Point3f& operator =(const Point3f&);
    Point3f operator +(const Point3f&) const;
    Point3f operator -(const Point3f&) const;
    Point3f operator *(float) const;
    Point3f operator /(float) const;
    
    inline void operator +=(const Point3f&);
    inline void operator -=(const Point3f&);
    inline void operator *=(float);
    inline void operator /=(float);
    
    inline float operator *(const Point3f&) const;
    
    // aggregators
    inline float angleXY() const { return atan2(y, x); }
    inline float angleYZ() const { return atan2(z, y); }
    inline float angleXZ() const { return atan2(z, x); }
    
    float magnitude() const;
    float distanceTo(Point3f& other);
    float angleTo(Point3f& other);
    
public:
    static float zero;
};

#endif
