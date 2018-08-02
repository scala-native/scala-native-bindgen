struct node {
    int value;
    struct node *next;
};

struct b;
struct c;

struct a {
    struct b *bb;
};

struct b {
    struct c **cc; // type will be replace with Ptr[Ptr[Byte]]
};

struct c {
    struct a aa;
};

/* function pointer type */

struct FuncPointerCycle2;

struct FuncPointerCycle1 {
    struct FuncPointerCycle2 *s;
};

struct FuncPointerCycle2 {
    struct FuncPointerCycle1 *(*memberFunction)(
        void); // type will be replace with CFunctionPtr0[Ptr[Byte]]
};

/* type has typedef alias */

struct TypeWithTypedef1;

typedef struct TypeWithTypedef1 TypeWithTypedef1;

struct TypeWithTypedef2 {
    TypeWithTypedef1 *s; // replaced with Ptr[Byte]
};

struct TypeWithTypedef1 {
    struct TypeWithTypedef2 *s;
};

/* two types should be replaced */

struct TwoTypesReplaced1;
struct TwoTypesReplaced2;

struct TwoTypesReplaced3 {
    struct TwoTypesReplaced1 *(*memberFunction)(struct TwoTypesReplaced2 *);
};

struct TwoTypesReplaced1 {
    struct TwoTypesReplaced2 *s;
};

struct TwoTypesReplaced2 {
    struct TwoTypesReplaced3 *s;
};

/* cycle contains union.
 * unions are represented as arrays therefore they cannot belong to Scala Native
 * cycle of types */

union cycleWithUnionU;

struct cycleWithUnionS {
    union cycleWithUnionU *u;
};

union cycleWithUnionU {
    struct cycleWithUnionS *s;
};

/* function pointer uses value type instead of pointer type */

struct FuncPointerWithValueType2;

struct FuncPointerWithValueType1 {
    struct FuncPointerWithValueType2 *s;
};

struct FuncPointerWithValueType2 {
    struct FuncPointerWithValueType1 (*memberFunction)(
        void); // return type will be replaced by CStruct0
};