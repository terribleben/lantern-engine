
// util_math.h

#ifndef __LANTERN_UTIL_MATH_H__
#define __LANTERN_UTIL_MATH_H__

#include <cstdlib>
#include <math.h>

#ifdef M_PI
#undef M_PI
#endif

#define M_PI 3.14159265358979323846

#ifdef M_2PI
#undef M_2PI
#endif

#define M_2PI (3.14159265358979323846 * 2.0)

// decide whether n is between a and b, inclusive.
bool between(float n, float a, float b);

#endif