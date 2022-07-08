#include "util.h"

int random_in_range(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
