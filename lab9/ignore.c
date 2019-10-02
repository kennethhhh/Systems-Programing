#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

void handler(int signum){
	printf("\nType 'quit' to exit program.\n");
}

int main(int argc, char const *argv[])
{
	char str[256];
	struct sigaction sa;
	sigset_t mask;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if(-1 == sigaction(SIGINT, &sa, NULL)){
		perror("sigaction");
		exit(-1);
	}


	while(1){
		scanf("%s", str);
		if (strcmp("quit\0", str) == 0){
			break;
		}
	}

	return 0;
}