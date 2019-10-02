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

    if(argc > 2 || argc == 1){
            printf("usage: tryit command\n");
            return status;
    }

    if((pid = fork()) < 0){
            status = -1;
    }

    else if(pid == 0){
            exit(execl(argv[1], argv[1], (char *)0));
    }

    else{
            wait(&status);
            if (WEXITSTATUS(status) == 0){
                    printf("Process %d succeeded.\n", pid);
            }
            else if(WEXITSTATUS(status) == 1){
                    printf("Process %d exited with an error value.\n", pid);
            }
            else if(WEXITSTATUS(status) == 255){
                    printf("%s: No such file or directory\n", argv[1]);
                    printf("Process %d exited with an error value.\n", pid);
            }
    }
    return status;

}

	