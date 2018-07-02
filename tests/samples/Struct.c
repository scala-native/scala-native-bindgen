#include "Struct.h"
#include <stdlib.h>

point_s getPoint() {
    point_s point = malloc(sizeof(struct point));
    point->x = 10;
    point->y = 20;
    return point;
}

int getBigStructSize() { return sizeof(struct bigStruct); }

char getCharFromAnonymousStruct(struct structWithAnonymousStruct *s) {
    return s->anonymousStruct.c;
}

char getIntFromAnonymousStruct(struct structWithAnonymousStruct *s) {
    return s->anonymousStruct.i;
}
