struct point {
    float x;
    float y;
};

struct vector {
    struct point a;
    struct point b;
};

struct vector *add(struct vector *v1, struct vector *v2);
