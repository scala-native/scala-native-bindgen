#define STRING "Hello, World!"

#define LONG 1000000000000l
#define LONG_WITHOUT_ENDING 1000000000000
#define LONG_LONG 1000000000000ll

/* maximum unsigned long does not fit into long type
 * therefore warning will be printed and definition will be ignored. */
#define MAXIMUM_UNSIGNED_LONG 18446744073709551615
#define MAXIMUM_SIGNED_LONG 9223372036854775807 // OK

#define MINIMUM_SIGNED_LONG -9223372036854775808           // OK
#define LESS_THEN_MINIMUM_SIGNED_LONG -9223372036854775809 // excluded

#define FLOAT 5.6

#define INT 42
#define MAXIMUM_INT 2147483647
#define NEW_INT INT
#define NEG_INT -INT

extern int a;
#define MY_A a // unsupported

#define wait_for_it(cond)                                                      \
    do {                                                                       \
        sleep(1000);                                                           \
    } while (!cond) // unsupported

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
