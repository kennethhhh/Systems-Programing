#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

void ticker(int signum){
	static int num = 0;
	if(++num%2){
		printf("Tick...\n");
	}
	else{
		printf("Tock\n");
	}
}

int main(int argc, char const *argv[])
{
	int i;
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
	itv.it_interval.tv_usec = 500000;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;
	if(-1 == setitimer(ITIMER_REAL, &itv, NULL)){
		perror("setitimer");
		exit(1);
	}

	for(i = 0; i < atoi(argv[1])*2; i++){
		pause();
	}

	printf("Time's up!\n");
	return 0;
}