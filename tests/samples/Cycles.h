struct b;
struct c;

struct a {
    struct b *bb;
};

struct b {
    struct c *cc; // type will be replace by Ptr[Byte]
};

struct c {
    struct a aa;
};
