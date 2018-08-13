#include "vector.h"
#include <stdlib.h>

struct vector *add(struct vector *v1, struct vector *v2) {
    struct vector *out = malloc(sizeof(struct vector));

    if (!out)
        return NULL;

    out->a.x = v1->a.x + v2->a.x;
    out->a.y = v1->a.y + v2->a.y;
    out->b.x = v1->b.x + v2->b.x;
    out->b.y = v1->b.y + v2->b.y;

    return out;
}