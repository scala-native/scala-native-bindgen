#ifndef _LIBSCBINTEST_H_
#define _LIBSCBINTEST_H_

    #define MAX_FOO  20

	struct Point {  
        int x;
        int y;
    };

    typedef struct Point Point;

    typedef struct {
        float l;
        char r;
    } foo;

    extern int sumi(int a, int b);
    extern double sumd(double a, double b);
    extern void doublePoint(Point* p);

#endif