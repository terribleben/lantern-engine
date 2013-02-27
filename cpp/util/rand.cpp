//
//  rand.cpp
//  lantern
//

#include "rand.h"

#define LANTERN_RAND_MAX 4294967294
#define LANTERN_RAND_A 1664525
#define LANTERN_RAND_C 1013904223

int rand(int seed) {
    return ((LANTERN_RAND_A * seed) + LANTERN_RAND_C) % LANTERN_RAND_MAX;
}

float randf(int seed) {
    return ((float)rand(seed) / (float)LANTERN_RAND_MAX);
}
