#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int one[2], two[2];
	pid_t child;

	pipe(one);
	pipe(two);

	child = fork();
	if (child == 0){
		close(1);
		dup2(one[1], 1);
		close(one[0]);
		exit(execlp(argv[1], argv[2], NULL));
	}

	close(0);
	dup2(one[0], 0);
	close(one[1]);
	execlp(argv[4], argv[4], NULL);
	exit(NULL);

	return 0;
}