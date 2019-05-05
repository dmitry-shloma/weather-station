#ifndef RANDHELPER_H
#define RANDHELPER_H

#include <stdlib.h>

/**
 * @brief init_rand function for initialization pseudo-random number generator algorithm
 * @param seed integer value to be used as seed by the algorithm
 */
void init_rand(unsigned int seed) {
    static bool is_init = false;
    if (!is_init) {
        srand(seed);
        is_init = true;
    }
}

/**
 * @brief rrand function that returns a random integer number in the range
 * @param min start of the range
 * @param max end of range
 * @return random integer number in the range
 */
int rrand(int min, int max) {
    return min + rand() % (max - min + 1);
}

/**
 * @brief brand function that returns a random "true" or "false"
 * @return a random "true" or "false"
 */
bool brand() {
    return rrand(0, 1);
}

/**
 * @brief rrand function that returns a random float number in the range
 * @param min start of the range
 * @param max end of range
 * @return random float number in the range
 */
float frrand(float min, float max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}

#endif // RANDHELPER_H

