//
//  Angle.h
//  lantern
//

#ifndef __LANTERN_ANGLE_H__
#define __LANTERN_ANGLE_H__

class Angle {
public:
    // return the difference (smallest solution) between angles a and b
    static float diff(float a, float b);
    
    static float toDegrees(float radians);
    static float toRadians(float degrees);
};

#endif
