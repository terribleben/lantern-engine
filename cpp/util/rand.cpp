//
//  rand.cpp
//  lantern
//

#include "rand.h"

#define LANTERN_RAND_MAX 4294967294
#define LANTERN_RAND_A 1664525
#define LANTERN_RAND_C 1013904223

unsigned int _lantern_rand_seed = LANTERN_RAND_A;

unsigned int lantern_rand() {
    unsigned int result = ((LANTERN_RAND_A * _lantern_rand_seed) + LANTERN_RAND_C) % LANTERN_RAND_MAX;
    _lantern_rand_seed = result;
    return result;
}

float lantern_randf() {
    return ((float)lantern_rand() / (float)LANTERN_RAND_MAX);
}

unsigned int lantern_rand(unsigned int seed) {
    _lantern_rand_seed = seed;
    return lantern_rand();
}

float lantern_randf(unsigned int seed) {
    _lantern_rand_seed = seed;
    return lantern_randf();
}
