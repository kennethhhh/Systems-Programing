#include "part6.h"

struct point create_point(double x, double y)
{
   struct point p = {x, y};
   return p;
}

int is_a_square(struct rectangle rect){
	int x = rect.bottomRight.x - rect.topLeft.x;
	int y = rect.topLeft.y - rect.bottomRight.y;
	if (x == y){
		return 1;
	}
	return 0;
}