//
//  Line3f.h
//  A line (or line segment) defined by points p and q.
//  Although p and q have three dimensions, the utility methods here operate in 2D unless otherwise noted.
//  lantern
//

#ifndef __LANTERN_LINE_H__
#define __LANTERN_LINE_H__

#include "Point3f.h"

class Line3f {
public:
    Point3f p, q;
    
public:
    Line3f() {  }
    Line3f(Point3f p, Point3f q) { this->p = p; this->q = q; }
    Line3f(const Line3f& other) { *this = other; }
    
    // decide whether we intersect the other line (in 2D).
    // if the intersection param is non-NULL, it will contain the point of intersection.
    bool intersect(Line3f& other, Point3f* intersection);
    
    // same as above, but the point of intersection must lie on the two segments for it to return true.
    bool intersectSegment(Line3f& other, Point3f* intersection);
    
    // given a Point3f a, return the Point3f on the line <- pq -> closest to a (in 2D).
    // the Point3f may or may not lie on segment pq.
    // this is the same as calculating the intersection of <- pq -> and the line through a normal to pq.
    Point3f nearestPointTo(Point3f& a);
    
    // determine whether a and b are on the same side of this line.
    bool sameSide(Point3f& a, Point3f& b);
    
    inline float slope() {
        if (p.x == q.x) return INFINITY;
        return (p.y - q.y) / (p.x - q.x);
    }
    
    inline float length() {
        return p.distanceTo(q);
    }
    
    inline float anglePQ() {
        return p.angleTo(q);
    }
    
public:
    Point3f& operator [] (unsigned int index);
    const Point3f& operator [] (unsigned int index) const;
    
    const Line3f& operator =(const Line3f&);
};

#endif
