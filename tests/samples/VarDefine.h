extern int a;
#define A a

extern const int constInt;
#define CONST_INT constInt

extern const int *constIntPointer;
#define CONST_INT_POINTER constIntPointer

#define B b // ignored because there is no such variable

extern int c;
#define C c
#undef C // removed

extern float f;
#define __PRIVATE f // should be filtered
