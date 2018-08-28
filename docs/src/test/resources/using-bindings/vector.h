struct point {
    float x;
    float y;
};

struct lineSegment {
    struct point a;
    struct point b;
};

float cosine(struct lineSegment *v1, struct lineSegment *v2);
