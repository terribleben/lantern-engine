//
//  PhysicalObject.h
//  lantern
//

#ifndef __LANTERN_WORLD_OBJECT_H__
#define __LANTERN_WORLD_OBJECT_H__

#include "Point3f.h"

#define PHYSICAL_OBJECT_MOTION_POSITION 0
#define PHYSICAL_OBJECT_MOTION_VELOCITY 1
#define PHYSICAL_OBJECT_MOTION_ACCELERATION 2

class PhysicalObject {
public:
    PhysicalObject() : orientation(0), radius(0) { }
    virtual ~PhysicalObject() { }
    
    Point3f position;
    Point3f velocity;
    Point3f acceleration;
    float orientation;
    float radius;
    
    virtual void step();
    virtual void draw();
    
    void setMotionFromVector(unsigned int motionComponent, float magnitude, float direction);
    void addMotionFromVector(unsigned int motionComponent, float magnitude, float direction);
    
protected:
    
};

#endif
