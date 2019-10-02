#include <stdlib.h>

int main(int argc, char const *argv[])
{
	char *space = malloc(1000000);
	free(space);
	return 0;
}