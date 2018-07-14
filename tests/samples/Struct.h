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

char getCharFromAnonymousStruct(struct structWithAnonymousStruct *s);

char getIntFromAnonymousStruct(struct structWithAnonymousStruct *s);

enum struct_op { STRUCT_SET, STRUCT_TEST };

int struct_test_long(struct bigStruct *s, enum struct_op op, long value);
int struct_test_double(struct bigStruct *s, enum struct_op op, double value);
int struct_test_point(struct bigStruct *s, enum struct_op op,
                      struct point *value);
