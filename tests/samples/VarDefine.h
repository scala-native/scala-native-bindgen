extern int a;
#define A a

#define B b // ignored because there is no such variable

extern int c;
#define C c
#undef C // removed

int getA();
