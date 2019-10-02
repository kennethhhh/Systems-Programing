#include "parseline.h"

#define READ_END 0
#define WRITE_END 1

/* GLOBAL VARIABLE */
int remaining_processes = 0;
extern int errno;

/* SIGINT HANDLER */
void exit_handler(int signum){
   /* WAIT FOR REMAINING PROCESSES TO COMPLETE */
   while(remaining_processes > 0){
      if(signum == SIGINT){
         wait(NULL);
         remaining_processes--;
      }
   }
   printf("\n");
}

int main(int argc, char *argv[]){
   int i, read_pipe[2], write_pipe[2], num_pipes;
   pid_t pid;
   struct command **cmd_arr;

   /* INITIALIZING READ PIPE */
   if(pipe(read_pipe) == -1){ 
      fprintf(stderr, "Pipe Failed" ); 
      exit(-1);
   }

   /* INITALIZING SIGNAL HANDLER */
   signal(SIGINT, exit_handler);

   /* READING ARGS FROM TERMINAL */
   while((cmd_arr = parseline(argc, argv, &num_pipes)) != NULL){
      /* CHECK FOR CD */
      if(strcmp(cmd_arr[0]->args[0], "cd\0") == 0){
         if(-1 == (chdir(cmd_arr[0]->args[1])))
            fprintf(stderr, "%s: %s\n", strerror(errno), cmd_arr[0]->args[1]);
         continue;
      }

      /* ELSE EXECUTE PROCESS */
      remaining_processes = num_pipes + 1;
      for (i = 0; i < num_pipes + 1; i++){
         /* INITIALIZE WRITE PIPE */
         if(pipe(write_pipe) == -1){ 
            fprintf(stderr, "Pipe Failed" ); 
            exit(-1);
         }

         /* CREATING CHILD PROCESS */
         if((pid = fork()) < 0){
            perror("fork");
            exit(-1);
         }

         /* CHILD PROCESS */
         if(pid == 0){ 

            /* CHECK FOR INPUT FILE REDIRECTION */
            if(i == 0 && cmd_arr[0]->change != 0 && cmd_arr[0]->input != 0){
               if(dup2(cmd_arr[0]->input, STDIN_FILENO) < 0){
                  perror("dup2");
                  exit(-1);
               }
            }

            /* CHECK FOR OUTPUT FILE REDIRECTION */
            if(i == num_pipes && cmd_arr[i]->change != 0 && cmd_arr[i]->output != 0){
               if(dup2(cmd_arr[i]->output, STDOUT_FILENO) < 0){
                  perror("dup2");
                  exit(-1);
               }
            }

            /* IF THERE ARE PIPES*/
            if(num_pipes > 0){ 

               /* FIRST PROCESS IN PIPELINE */
               if(i == 0 && cmd_arr[0]->output == 0){ 
                  if(dup2(write_pipe[WRITE_END], STDOUT_FILENO) < 0){
                     perror("dup2");
                     exit(-1);
                  }
               }
               /* MIDDLE PROCESS IN PIPELINE */
               else if(i < num_pipes){ 
                  if(dup2(read_pipe[READ_END], STDIN_FILENO) < 0){
                     perror("dup2");
                     exit(-1);
                  }
                  if(dup2(write_pipe[WRITE_END], STDOUT_FILENO) < 0){
                     perror("dup2");
                     exit(-1);
                  }
               }
               /* LAST PROCESS IN PIPELINE */
               else
               {
                  if(dup2(read_pipe[READ_END], STDIN_FILENO) < 0){
                     perror("dup2");
                     exit(-1);
                  }
               }
            }
            
            /* CLOSE PIPES */
            close(read_pipe[READ_END]);
            close(read_pipe[WRITE_END]);
            close(write_pipe[READ_END]);
            close(write_pipe[WRITE_END]);
            
            /* EXEC THE PROCESS */
            if((execvp(cmd_arr[i]->args[0], format_args(cmd_arr[i]->args, cmd_arr[i]->argc))) == -1){
               perror("execvp");
               exit(-1);
            }
            exit(0);
         }

         /* PARENT PROCESS */

         /* REDIRECT PIPES TO EACH OTHER */
         close(read_pipe[READ_END]);
         close(read_pipe[WRITE_END]);
         read_pipe[0] = write_pipe[0];
         read_pipe[1] = write_pipe[1];
      }

      /* WAIT FOR CHILDREN TO FINISH */
      for(i = 0; i < num_pipes + 1; i++){
         wait(NULL);
         remaining_processes--;
      }
      /*fflush(stdout);*/

      /* CLOSE PARENT FILE DESCRIPTORS */
      /*
      for(i = 0; i < 10; i++){
         if(cmd_arr[i]->input != 0){
            close(cmd_arr[i]->input);   
         }
         if(cmd_arr[i]->output != 0){
            close(cmd_arr[i]->output);   
         }
      }
      */
   }
   printf("\n");

   free(cmd_arr);
   return 0;
}
