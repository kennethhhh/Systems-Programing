#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	pid_t pid;
	int status;
	printf("Hello, world!\n");
	pid = fork();

	if (pid == 0){
		printf("This is the child, pid %d\n", getpid());
	}
	else{
		printf("This is the parent, pid %d\n", getpid());
		wait(&status);
		printf("This is the parent, pid %d, signing off\n", getpid());
	}
	exit(0);
	return 0;
}