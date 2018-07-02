struct point {
    int x;
    int y;
};

typedef struct point *point_s;

point_s getPoint();

struct bigStruct {
    long one;
    char two;
    int three;
    float four;
    double five;
    point_s six;
    int seven;
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
