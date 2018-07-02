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

struct structWithAnonymousStruct {
    int a;
    struct {
        char c;
        int i;
    } anonymousStruct;
};

char getCharFromAnonymousStruct(struct structWithAnonymousStruct *s);

char getIntFromAnonymousStruct(struct structWithAnonymousStruct *s);
