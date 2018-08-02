#include "Union.h"
#include <stdlib.h>
#include <string.h>

int union_get_sizeof() { return sizeof(union values); }

int union_test_int(union values *v, enum union_op op, int value) {
    switch (op) {
    case UNION_SET:
        v->i = value;
        return 1;
    case UNION_TEST:
        return v->i == value;
    }
}

int union_test_long(union values *v, enum union_op op, long value) {
    switch (op) {
    case UNION_SET:
        v->l = value;
        return 1;
    case UNION_TEST:
        return v->l == value;
    }
}

int union_test_long_long(union values *v, enum union_op op, long long value) {
    switch (op) {
    case UNION_SET:
        v->ll = value;
        return 1;
    case UNION_TEST:
        return v->ll == value;
    }
}

int union_test_double(union values *v, enum union_op op, double value) {
    switch (op) {
    case UNION_SET:
        v->d = value;
        return 1;
    case UNION_TEST:
        return v->d == value;
    }
}

int union_test_string(union values *v, enum union_op op, const char *value) {
    switch (op) {
    case UNION_SET:
        v->s = value;
        return 1;
    case UNION_TEST:
        return v->s == value || !strcmp(v->s, value);
    }
}

int union_test_struct(union values *v, enum union_op op, struct s *value) {
    switch (op) {
    case UNION_SET:
        v->structInUnion.a = value->a;
        return 1;
    case UNION_TEST:
        return v->structInUnion.a == value->a;
    }
}
