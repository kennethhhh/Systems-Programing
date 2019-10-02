#include <stdio.h>

#include "checkit.h"
#include "part2.h"

void test_calc_1()
{
   checkit_double(calc(-1.0), 40);
   checkit_double(calc(12.0), 49);
   checkit_double(calc(-12.0), 31);
}

void test_calc()
{
   test_calc_1();
}

int main(void)
{
   test_calc();

   return 0;
}
