//
//  Line3f.cpp
//  lantern
//

#include "Line3f.h"
#include "util_math.h"

Point3f& Line3f::operator [] (unsigned int index) {
    return *(Point3f*)(&p + sizeof(Point3f) * index);
}

const Point3f& Line3f::operator [] (unsigned int index) const {
    return *(Point3f*)(&p + sizeof(Point3f) * index);
}

const Line3f& Line3f::operator =(const Line3f& other) {
    p = other.p;
    q = other.q;
    return *this;
}

bool Line3f::intersect(Line3f& other, Point3f* intersection) {
    float m1 = this->slope();
    float m2 = other.slope();
    
    if (m1 != m2) {
        float intX, intY;
        
        if (m1 == INFINITY) {
            intX = p.x;
            intY = m2 * (intX - other.p.x) + other.p.y;
        } else if (m2 == INFINITY) {
            intX = other.p.x;
            intY = m1 * (intX - p.x) + p.y;
        } else {
            intX = (-p.y + (m1 * p.x) + other.p.y - (m2 * other.p.x)) / (m1 - m2);
            intY = m1 * (intX - p.x) + p.y;
        }
        
        if (intersection)
            intersection->set(intX, intY, 0);
        
        return true;
    }
    return false;
}

bool Line3f::intersectSegment(Line3f& other, Point3f* intersection) {
    Point3f localIntersection;
    bool intersected = intersect(other, &localIntersection);
    if (intersected) {
        if (
            between(localIntersection.x, p.x, q.x)
            && between(localIntersection.x, other.p.x, other.q.x)
            && between(localIntersection.y, p.y, q.y)
            && between(localIntersection.y, other.p.y, other.q.y)
            ) {
            if (intersection)
                *intersection = localIntersection;
            return true;
        }
        return false;
    }
    return false;
}

Point3f Line3f::nearestPointTo(Point3f& a) {
    Point3f result;
    
    if (p.x == q.x) {
        result.x = p.x;
        result.y = a.y;
    } else if (p.y == q.y) {
        result.x = a.x;
        result.y = p.y;
    } else {
        float m = slope();
        float mInv = 1.0f / m;
        result.x = (m * p.x - p.y + a.y + mInv * a.x) / (m + mInv);
        result.y = m * (result.x - p.x) + p.y;
    }
    return result;
}

bool Line3f::sameSide(Point3f &a, Point3f &b) {
    Point3f AB = b - a, AP = p - a, AQ = q - a;
    
    Point3f cross1 = Point3f::crossProduct(AB, AP);
    Point3f cross2 = Point3f::crossProduct(AB, AQ);
    
	return ((cross1.x * cross2.x + cross1.y * cross2.y + cross1.z * cross2.z) >= 0);
}
