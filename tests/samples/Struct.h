struct point {
    int x;
    int y;
};

typedef struct point point;

struct points {
    struct point p1;
    point p2;
};

enum pointIndex { X1, Y1, X2, Y2 };

void setPoints(struct points *points, int x1, int y1, int x2, int y2);

int getPoint(struct points *points, enum pointIndex pointIndex);

typedef struct point *point_s;

point_s createPoint();

struct bigStruct {
    long one;
    char two;
    int three;
    float four;
    double five;
    struct point six;
    struct point *seven;
    int eight;
    int nine;
    int ten;
    int eleven;
    int twelve;
    int thirteen;
    int fourteen;
    int fifteen;
    int sixteen;
    int seventeen;
    int eighteen;
    int nineteen;
    int twenty;
    int twentyOne;
    int twentyTwo;
    int twentyThree;
};

int getBigStructSize();

struct structWithAnonymousStruct {
    int a;
    struct {
        char c;
        int i;
    } anonymousStruct;
};

struct __attribute__((__packed__)) packedStruct { // no helper methods
    char a;
};

struct bitFieldStruct { // no helper methods
    unsigned char b1 : 3;
    unsigned char : 0; // start a new byte
    unsigned char b2 : 6;
    unsigned char b3 : 2;
};

struct bitFieldOffsetDivByEight { // no helper methods
    unsigned b1 : 8;
    unsigned b2 : 8;
    unsigned char b3 : 8;
};

char getCharFromAnonymousStruct(struct structWithAnonymousStruct *s);

char getIntFromAnonymousStruct(struct structWithAnonymousStruct *s);

enum struct_op { STRUCT_SET, STRUCT_TEST };

int struct_test_char(struct bigStruct *s, enum struct_op op, char value);
int struct_test_int(struct bigStruct *s, enum struct_op op, int value);
int struct_test_long(struct bigStruct *s, enum struct_op op, long value);
int struct_test_double(struct bigStruct *s, enum struct_op op, double value);
int struct_test_point(struct bigStruct *s, enum struct_op op,
                      struct point *value);
