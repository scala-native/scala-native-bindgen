struct point;
struct point *move(struct point *point, int x, int y);

typedef struct points points;
typedef union u u;

union u *processPoints(points *p);

union u {
    int i;
    float f;
};

struct points {
    struct point *point1;
    struct point *point2;
};

struct point {
    int x;
    int y;
};
