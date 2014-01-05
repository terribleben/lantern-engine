//
//  Point.cpp
//  lantern
//

#include "Point3f.h"

float Point3f::zero = 0.0f;

Point3f::Point3f(float _x, float _y, float _z) {
    set(_x, _y, _z);
}

Point3f::Point3f(const Point3f& other) {
     *this = other;
}

void Point3f::set(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

void Point3f::setAll(float val) {
    x = y = z = val;
}

float& Point3f::operator [] (unsigned int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    return zero;
}

const float& Point3f::operator [] (unsigned int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    return zero;
}

const Point3f& Point3f::operator =(const Point3f& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

Point3f Point3f::operator +(const Point3f& other) const {
    Point3f result = *this;
    result += other;
    return result;
}

Point3f Point3f::operator -(const Point3f& other) const {
    Point3f result = *this;
    result -= other;
    return result;
}

Point3f Point3f::operator *(float scalar) const {
    Point3f result = *this;
    result *= scalar;
    return result;
}

Point3f Point3f::operator /(float scalar) const {
    Point3f result = *this;
    result /= scalar;
    return result;
}

inline void Point3f::operator +=(const Point3f& other) {
    x += other.x; y += other.y; z += other.z;
}

inline void Point3f::operator -=(const Point3f& other) {
    x -= other.x; y -= other.y; z -= other.z;
}

const bool Point3f::operator ==(const Point3f& other) {
    return (x == other.x && y == other.y && z == other.z);
}

inline void Point3f::operator *=(float scalar) {
     x *= scalar; y *= scalar; z *= scalar;
}

inline void Point3f::operator /=(float scalar) {
    x /= scalar; y /= scalar; z /= scalar;
}

inline float Point3f::operator *(const Point3f& other) const {
    float result = x * other.x + y * other.y + z * other.z;
    return result;
}

float Point3f::magnitude() const {
    return sqrt( x*x + y*y + z*z );
}

void Point3f::normalize() {
    float m = magnitude();
    if (m != 0)
        *this *= 1.0f / m;
}

float Point3f::distanceTo(Point3f &other) {
    return sqrt(pow((x - other.x), 2.0) + pow((y - other.y), 2.0));
}

float Point3f::angleTo(Point3f& other) {
    return atan2(other.y - y, other.x - x);
}
