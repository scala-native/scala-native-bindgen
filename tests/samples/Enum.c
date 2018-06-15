#include "Enum.h"

enum days get_WEDNESDAY() { return WEDNESDAY; }

char *check_BIG_NEG_A(enum bigNegativeValues big_neg_a) {
    if (big_neg_a == BIG_NEG_A) {
        return "OK";
    }
    return "FAIL";
}
