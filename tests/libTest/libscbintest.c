#include "libscbintest.h"

int sumi(int a, int b){
	return a + b;
}

double sumd(double a, double b){
	return a + b;
}

void doublePoint(Point* p) {
	p->x *= 2;
	p->y *= 2;
}