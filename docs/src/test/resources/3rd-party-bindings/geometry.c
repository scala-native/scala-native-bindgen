#include "geometry.h"
#include <math.h>

float circle_area(struct circle *circle) {
    return pow(circle->radius, 2) * M_PI;
}
