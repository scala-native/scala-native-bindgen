#include "Struct.h"
#include <stdlib.h>

void setPoints(struct points *points, int x1, int y1, int x2, int y2) {
    points->p1.x = x1;
    points->p1.y = y1;
    points->p2.x = x2;
    points->p2.y = y2;
}

int getPoint(struct points *points, enum pointIndex pointIndex) {
    switch (pointIndex) {
    case X1:
        return points->p1.x;
    case X2:
        return points->p2.x;
    case Y1:
        return points->p1.y;
    case Y2:
        return points->p2.y;
    }
}

point_s createPoint() {
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

int struct_test_long(struct bigStruct *s, enum struct_op op, long value) {
    switch (op) {
    case STRUCT_SET:
        s->one = value;
        return 1;
    case STRUCT_TEST:
        return s->one == value;
    }
}

int struct_test_double(struct bigStruct *s, enum struct_op op, double value) {
    switch (op) {
    case STRUCT_SET:
        s->five = value;
        return 1;
    case STRUCT_TEST:
        return s->five == value;
    }
}

int struct_test_point(struct bigStruct *s, enum struct_op op,
                      struct point *value) {
    switch (op) {
    case STRUCT_SET:
        s->six = *value;
        return 1;
    case STRUCT_TEST:
        return (s->six.x == value->x) && (s->six.y == value->y);
    }
}
