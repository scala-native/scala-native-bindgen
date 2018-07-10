void func(); // removed

extern int mode; // removed

typedef int size;

struct metadata {
    unsigned year;
    char *publisher;
};

typedef struct metadata metadata;

struct document {
    metadata m;
};

typedef struct document unusedTypedef; // removed

union data {
    int i;
    float f;
}; // removed

union data getData(); // removed

enum { ONE, TWO }; // anonymous enum will be removed

enum unusedEnum { UNUSED }; // removed

typedef enum unusedEnum unusedEnum; // removed

struct unusedStruct {
    enum unusedEnum e;
}; // removed

enum semester { AUTUMN, SPRING };
