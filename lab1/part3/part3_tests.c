#include <stdio.h>

#include "checkit.h"
#include "part3.h"

void test_sum_1()
{
	int yayeet[] = {1,2,3,4,6};
	int ok[] = {10,15};
   checkit_int(sum(yayeet, 5), 16);
   checkit_int(sum(ok, 0), 0);
}

void test_sum()
{
   test_sum_1();
}

int main(void)
{
   test_sum();

   return 0;
}
