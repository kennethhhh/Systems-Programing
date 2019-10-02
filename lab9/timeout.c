#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t killpid;

void ticker(int signum){
	printf("K*lling child\n");
	kill(killpid, SIGINT);
	exit(-1);
}

int main(int argc, char const *argv[])
{
	int i, status;
	pid_t child;
	struct sigaction sa;
	struct itimerval itv;

	sa.sa_handler = ticker;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(-1 == sigaction(SIGALRM, &sa, NULL)){
		perror("sigaction");
		exit(1);
	}

	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = atoi(argv[1]);
	itv.it_value.tv_usec = 0;
	if(-1 == setitimer(ITIMER_REAL, &itv, NULL)){
		perror("setitimer");
		exit(1);
	}
	
	child = fork();
	killpid = child;

	if(child == 0){
		printf("Child exec\n");
		if(-1 == execlp(argv[2], argv[2], argv[3], NULL)){
			perror("exec");
			exit(-1);
		}
	}


	wait(&status);
	if(WIFEXITED(status)){
		printf("exit status: %d\n", WEXITSTATUS(status));
		exit(status);
	}

	return 0;
}