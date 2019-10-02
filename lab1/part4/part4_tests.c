#include <stdio.h>

#include "checkit.h"
#include "part4.h"

void test_str_lower_1()
{
	char buff[6];
   	checkit_string(str_lower("HELLO", buff), "hello");
}

void test_str_lower_mutate_1()
{
   char string[] = "DEEZnutz";
   str_lower_mutate(string);
   checkit_string(string, "deeznutz");
}

void test_str_lower()
{
   test_str_lower_1();
}

void test_str_lower_mutate()
{
   test_str_lower_mutate_1();
}

int main(void)
{
   test_str_lower();
   test_str_lower_mutate();

   return 0;
}
