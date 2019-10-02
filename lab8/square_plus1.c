#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int input, value;
	int one[2];
	int two[2];
	int three[2];
	pid_t child1, child2;

	pipe(one);
	pipe(two);
	pipe(three);
	
	while(1){
		printf("Enter a value:");
		input = scanf("%d", &value);
		if(input == EOF){
			break;
		}
		write(one[1], &value, sizeof(int));
		child1 = fork();
		child2 = fork();
		if(child1 == 0){
			close(one[1]);
			close(two[0]);
			/*
			read(one[0], &value, sizeof(int));
			value *= value;
			write(two[1], &value, sizeof(int));
			*/
			dup2(one[0], 0);
			read(0, &value, sizeof(int));
			value *= value;
			write(1, &value, sizeof(int));
			exit(0);
		}
		if(child2 == 0){
			read(two[0], &value, sizeof(int));
			value += 1;
			write(three[1], &value, sizeof(int));
			exit(0);
		}
		else{
			read(three[0],&value,sizeof(int));
			printf("square_plus1: %d\n", value);
		}
	}
	close(one[1]);
	close(two[1]);
	return 0;
}