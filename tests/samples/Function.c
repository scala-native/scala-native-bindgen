#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "Function.h"

int no_args() { return 42; }

float void_arg(void) { return 1.5; }

char one_arg(int a) { return (char)a; }

void *two_args(float a, int b) {
    static char buf[128];
    if (snprintf(buf, sizeof(buf), "%.2f %d", a, b) > sizeof(buf))
        return "oops";
    return buf;
}

double anonymous_args(float a, int b) { return a + b; }

double variadic_args(double a, char *varArgs, ...) {
    va_list args;

    va_start(args, varArgs);
    for (; *varArgs; varArgs++)
        a += (*varArgs - '0') * va_arg(args, int);
    return a;
}
