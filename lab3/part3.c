#include <unistd.h>
#include <limits.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	long stuff = sysconf(_SC_CHILD_MAX);
	printf("%ld\n", stuff);

	stuff = sysconf(_SC_OPEN_MAX);
	printf("%ld\n", stuff);

	stuff = sysconf(_SC_PAGESIZE);
	printf("%ld\n", stuff);
	return 0;
}