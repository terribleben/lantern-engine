//
//  Angle.cpp
//  lantern
//

#include "Angle.h"
#include "util_math.h"

float Angle::diff(float a, float b) {
    while (a > M_2PI) a -= M_2PI;
	while (b > M_2PI) b -= M_2PI;
	while (a < 0) a += M_2PI;
	while (b < 0) b += M_2PI;
	
	float diff = a - b;
	if (fabs(diff) <= M_PI) return diff;
	
	while (a > M_PI) a -= M_2PI;
	while (b > M_PI) b -= M_2PI;
	while (a < -M_PI) a += M_2PI;
	while (b < -M_PI) b += M_2PI;
    
	return a - b;
}

float Angle::toDegrees(float radians) {
    return radians * (180.0f / M_PI);
}

float Angle::toRadians(float degrees) {
    return degrees * (M_PI / 180.0f);
}
