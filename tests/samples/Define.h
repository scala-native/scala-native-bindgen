#define STRING "Hello, World!"

#define INT 42
// #define LONG 10000000000 // this will fail
#define FLOAT 5.6

#define NEW_INT INT // unsupported

extern int a;
#define MY_A a // unsupported

#ifdef NOT_DEFINED
#define SHOULD_NOT_BE_DEFINED 0
#endif

#if INT == 0
#define SHOULD_NOT_BE_DEFINED 1
#else
#define SHOULD_BE_DEFINED 2
#endif
