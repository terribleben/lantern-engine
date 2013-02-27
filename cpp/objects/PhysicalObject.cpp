//
//  PhysicalObject.cpp
//  lantern
//

#include "PhysicalObject.h"


void PhysicalObject::step() {
    velocity += acceleration;
    position += velocity;
}

void PhysicalObject::draw() {
    
}

void PhysicalObject::setMotionFromVector(unsigned int motionComponent, float magnitude, float direction) {
    Point3f result(magnitude * cos(direction), magnitude * sin(direction), 0);
    
    switch (motionComponent) {
        case PHYSICAL_OBJECT_MOTION_POSITION: {
            position = result;
            break;
        }
        case PHYSICAL_OBJECT_MOTION_VELOCITY: {
            velocity = result;
            break;
        }
        case PHYSICAL_OBJECT_MOTION_ACCELERATION: {
            acceleration = result;
            break;
        }
    }
}

void PhysicalObject::addMotionFromVector(unsigned int motionComponent, float magnitude, float direction) {
    Point3f result(magnitude * cos(direction), magnitude * sin(direction), 0);
    
    switch (motionComponent) {
        case PHYSICAL_OBJECT_MOTION_POSITION: {
            position += result;
            break;
        }
        case PHYSICAL_OBJECT_MOTION_VELOCITY: {
            velocity += result;
            break;
        }
        case PHYSICAL_OBJECT_MOTION_ACCELERATION: {
            acceleration += result;
            break;
        }
    }
}