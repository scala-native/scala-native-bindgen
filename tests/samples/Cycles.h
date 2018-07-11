struct b;
struct c;

struct a {
    struct b *bb;
};

struct b {
    struct c *cc;
};

struct c {
    struct a aa;
};
