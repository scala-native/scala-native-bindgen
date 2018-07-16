int no_args();
float void_arg(void);
char one_arg(int a);
void *two_args(float a, int b);
double anonymous_args(float, int);
double variadic_args(double a, char *varArgs, ...);

struct s {
    int val;
};

void acceptsStructValue(struct s); // function is skipped with warning

typedef struct s s;

s returnsStructValue(); // function is skipped with warning

union u {
    int a;
};

void acceptsUnionValue(union u); // function is skipped with warning

void acceptsArray(int[10]); // it's okay because the type is pointer to int
