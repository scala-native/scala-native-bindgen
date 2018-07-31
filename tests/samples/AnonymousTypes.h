/* structs */

void foo(struct { char a; } * s);

struct StructWithAnonymousStruct {
    struct {
        union {
            long a;
        } * innerUnion;
    } * innerStruct;

    enum { A, B, C } innerEnum;
};

struct {
    int result;
} * bar();
