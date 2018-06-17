#define STRING "Hello, World!"

#define LONG 1000000000000l
#define LONG_WITHOUT_ENDING 1000000000000
#define LONG_LONG 1000000000000ll

/* maximum unsigned long does not fit into long type
 * therefore warning will be printed and definition will be ignored. */
#define MAXIMUM_UNSIGNED_LONG 18446744073709551615
#define MAXIMUM_SIGNED_LONG 9223372036854775807 // OK

/* negative values are currently ignored because there are 2 tokens in the
 * representation */
// #define MINIMUM_SIGNED_LONG -9223372036854775808

#define FLOAT 5.6

#define INT 42
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
