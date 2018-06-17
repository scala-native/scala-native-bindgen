#define STRING "Hello, World!"

#define INT 42
// #define LONG 10000000000 // this will fail
#define FLOAT 5.6

#define NEW_INT INT // unsupported

extern int a;
#define MY_A a // unsupported

#ifdef NOT_DEFINED
#define SHOULD_NOT_BE_DEFINED "Because NOT_DEFINED is not defined"
#endif

#define DEFINED_ONLY_IN_HEADER "Defined only inside the header file"

#if INT == 0
#define SHOULD_NOT_BE_DEFINED "Because INT is 42"
#else
#define SHOULD_BE_DEFINED "Because INT is not equal to 0"
#endif

#undef DEFINED_ONLY_IN_HEADER
