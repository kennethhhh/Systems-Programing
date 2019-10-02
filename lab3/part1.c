#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 2048
/*
2
16
64
128
256
512
1024
2048
*/

int main(int argc, char const *argv[])
{
	clock_t start, end;
	double time;
	char buffer[SIZE];
	int n, file, i;

	start = clock();

	file = open("/usr/lib/locale/locale-archive", O_RDONLY);
	while((n = read(file, buffer, SIZE)) > 0);
	close(file);

	end = clock();
	time = ((double)(end - start))/CLOCKS_PER_SEC;
	printf("Time it takes: %f\n", time);
	return 0;
}