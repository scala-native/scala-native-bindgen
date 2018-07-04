union values {
    long l;
    int i;
    long long ll;
    double d;
    const char *s;
};

enum union_op { UNION_SET, UNION_TEST };

int union_get_sizeof();

int union_test_int(union values *v, enum union_op op, int value);
int union_test_long(union values *v, enum union_op op, long value);
int union_test_long_long(union values *v, enum union_op op, long long value);
int union_test_double(union values *v, enum union_op op, double value);
int union_test_string(union values *v, enum union_op op, const char *value);
