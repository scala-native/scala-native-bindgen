#include "Struct.h"
#include "include/CustomNames.h"

void useStruct(struct point *);

point_s returnTypedef_point_s(); // struct point will be references instead of
                                 // point_s because clang replaces the typedef
point *returnTypedef_point();

typedef struct bigStruct aliasForBigStruct;

struct usesImportedEnum {
    enum pointIndex index;
    union weight weight;
};

void readBook(struct book *book);
void getWeight(union weight *weight);

myInt getMyInt();
enumWithTypedef getEnum();