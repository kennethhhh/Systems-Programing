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
	int n;
	FILE *file;

	start = clock();

	file = fopen("/usr/lib/locale/locale-archive", "r");
	while((n = fread(buffer, 1, SIZE, file)) > 0);
	fclose(file);

	end = clock();
	time = ((double)(end - start))/CLOCKS_PER_SEC;
	printf("Time it takes: %f\n", time);
	return 0;
}