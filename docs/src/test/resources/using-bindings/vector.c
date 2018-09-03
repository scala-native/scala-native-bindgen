#include "vector.h"
#include <math.h>
#include <stdlib.h>

float cosine(struct lineSegment *v1, struct lineSegment *v2) {
    float v1x = fabsf(v1->b.x - v1->a.x);
    float v1y = fabsf(v1->b.y - v1->a.y);
    float v1Length = sqrtf(v1x * v1x + v1y * v1y);
    float v2x = fabsf(v2->b.x - v2->a.x);
    float v2y = fabsf(v2->b.y - v2->a.y);
    float v2Length = sqrtf(v2x * v2x + v2y * v2y);
    return (v1x * v2x + v1y * v2y) / (v1Length * v2Length);
}
