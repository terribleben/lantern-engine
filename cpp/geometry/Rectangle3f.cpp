//
//  Rectangle3f.cpp
//  lantern
//

#include "Rectangle3f.h"
#include "Triangle3f.h"
#include "util_math.h"

#include <cstdlib>

Rectangle3f::Rectangle3f(Point3f origin, Point3f size, float orientation) {
    corners = NULL;
    
    this->origin = origin;
    this->size = size;
    if (orientation != 0)
        this->setOrientation(orientation);
    else
        this->orientation = orientation;
}

Rectangle3f::Rectangle3f(Point3f bottomLeft, Point3f topRight) {
    orientation = 0;
    corners = NULL;
    origin = (bottomLeft + topRight) * 0.5f;
    size = topRight - bottomLeft;
}

Rectangle3f::~Rectangle3f() {
    if (corners) {
        free(corners);
        corners = NULL;
    }
}

Point3f* Rectangle3f::getCorners() {
    if (!corners)
        setOrientation(this->orientation);
    return corners;
}

void Rectangle3f::setOrientation(float orientation) {
    if (!corners) {
        corners = (Point3f*) malloc(sizeof(Point3f) * 4);
    }
    
    while (orientation > M_2PI)
        orientation -= M_2PI;
    while (orientation < 0)
        orientation += M_2PI;
    
    float diagAngle = size.angleXY();
    float diagLength = size.magnitude() * 0.5f;
    
    // corners in a z shape, top to bottom.
    corners[0].set(origin.x + diagLength * cos(M_PI - diagAngle + orientation), origin.y + diagLength * sin(M_PI - diagAngle + orientation), 0);
    corners[1].set(origin.x + diagLength * cos(diagAngle + orientation), origin.y + diagLength * sin(diagAngle + orientation), 0);
    corners[2].set(origin.x + diagLength * cos(M_PI + diagAngle + orientation), origin.y + diagLength * sin(M_PI + diagAngle + orientation), 0);
    corners[3].set(origin.x + diagLength * cos(-diagAngle + orientation), origin.y + diagLength * sin(-diagAngle + orientation), 0);
    
    this->orientation = orientation;
}

float Rectangle3f::getOrientation() {
    return orientation;
}

bool Rectangle3f::contains(Point3f& p) {
    if (!corners || orientation == 0 || orientation == M_PI) {
        float halfWidth = size.x * 0.5f, halfHeight = size.y * 0.5f;
        return (p.x >= origin.x - halfWidth && p.x <= origin.x + halfWidth && p.y >= origin.y - halfHeight && p.y <= origin.y + halfHeight);
    } else {
        return (
                   Triangle3f(corners[2], corners[1], corners[0]).contains(p)
                || Triangle3f(corners[3], corners[2], corners[1]).contains(p)
            );
    }
}

const Rectangle3f& Rectangle3f::operator =(const Rectangle3f& other) {
    origin = other.origin;
    size = other.size;
    if (other.orientation != 0) {
        setOrientation(other.orientation); // this could be a straight copy, rather than recalculating corners.
    }
    return *this;
}
