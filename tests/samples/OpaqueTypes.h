#include "include/OpaqueTypes.h"

typedef struct points points;

struct point;
struct point *move(struct point *point, int x, int y);

typedef union u u;

union u *processPoints(points *p);

union u {
    int i;
    float f;
};

struct points {
    struct point *point1;
    struct point *point2;
};

struct point {
    int x;
    int y;
};

struct undefinedStruct;

void usePointerToUndefinedStruct(struct undefinedStruct *);

struct structWithPointerToUndefinedStruct {
    struct undefinedStruct *field;
};

union unionWithPointerToUndefinedStruct {
    struct undefinedStruct *field;
};

typedef union undefinedUnion undefinedUnion;

typedef undefinedUnion *aliasToPointerOfUndefinedUnion;

aliasToPointerOfUndefinedUnion *fun();

typedef struct undefinedStruct aliasForUndefinedStruct; // okay

aliasForUndefinedStruct *returnPointerToAliasOfUndefinedStruct();

void usePointerToUndefinedIncludedStruct(undefinedIncludedStruct *);

typedef aliasToPointerOfUndefinedUnion (
    *functionPointerWithPointerToOpaqueType)(struct undefinedStruct **);

void useUndefinedStruct(
    struct undefinedStruct); // removed. Error message is printed

extern struct undefinedStruct removedExtern; // removed

#define removedExternAlias removedExtern // removed
