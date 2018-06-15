#include "Struct.h"
#include <stdlib.h>

point_s getPoint() {
    point_s point = malloc(sizeof(struct point));
    point->x = 10;
    point->y = 20;
    return point;
}
