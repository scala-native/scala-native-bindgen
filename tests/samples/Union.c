#include "Union.h"
#include <stdlib.h>

union values *getValues() {
    union values *myValues = malloc(sizeof(union values));
    myValues->a = 10;
    return myValues;
}
