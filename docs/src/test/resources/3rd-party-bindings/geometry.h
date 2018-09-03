#include "../using-bindings/vector.h"

//#using-struct-point
struct circle {
    struct point point;
    double radius;
};
//#using-struct-point

float circle_area(struct circle *circle);
