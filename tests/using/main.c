#include "../libTest/libscbintest.h"

#include <stdio.h>

int main(){
	printf("%d\n", sumi(1,2));
	printf("%lf\n", sumd(3.f,4.f));
	Point p;
	p.x = 2;
	p.y = 3;
	doublePoint(&p);
	printf("x:%d y:%d\n", p.x, p.y);
	return 0;
}