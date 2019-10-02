#include <stdio.h>

#include "checkit.h"
#include "part1.h"

void test_swap_1()
{
	int x = 1, y = 2
	int *first, *second;
	first = &x;
	second = &y;
	swap(first,second);

   checkit_int(x, 2);
   checkit_int(y, 1);
}

void test_swap()
{
   test_swap_1();
}

int main(void)
{
   test_swap();

   return 0;
}
