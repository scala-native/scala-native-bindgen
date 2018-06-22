extern int a;
#define A a

#define B b // ignored because there is no such variable

extern int c;
#define C c
#undef C // removed

extern float f;
#define __PRIVATE f // should be filtered

int getA();
