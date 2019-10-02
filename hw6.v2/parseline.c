#include "parseline.h"

/*
CHANGE RETURN NULL FOR ERROR HANDLING SO SHELL DOES NOT EXIT
*/

struct command ** parseline(int argc, char *argv[], int *num_pipes)
{
   struct command **cmd_arr = (struct command **)calloc(10, sizeof(struct command *));
   struct command *cur;
   char cmd[1028];
   char *line;
   int stage = 0;
   int in_flag = 0, out_flag = 0;
   *num_pipes = 0;

   printf("8-P ");
   if(scanf("%[^\n]%*c", cmd) == EOF) /* HANDLE OVERFLOW */
      return NULL;

   if(strlen(cmd) == 0) /* EMPTY LINE */
   {
      printf("usage: parseline [ -c ] [ -d ] set1 [ set2 ]\n");
      /*exit(-1);*/
      return NULL;
   }
   if(strlen(cmd) > 512)
   {
      printf("Command too long.\n");
      /*exit(-1);*/
      return NULL;
   }

   line = strtok(cmd, " "); /* COMMAND */
   cur = create_command(line, stage);
   while(1){
      /* parse here */
      line = strtok(NULL, " "); /* GET NEXT INPUT */
      if(line == NULL)
      {
         cmd_arr[stage] = cur; 
         break;
      }

      if(strcmp(line, "|") == 0){
         (*num_pipes)++;
      }

      if(strcmp(line, "<") == 0) /* INPUT */
      {
         if(stage > 0) /* changing stdin in a child process */
         {
            printf("Ambiguous input\n");
            /*exit(-1);*/
            return NULL;
         }

         if(in_flag == 0) /* VALID INPUT */
         {
            strcat(cur->name, " < ");
            line = strtok(NULL, " ");

            strcat(cur->name, line);
            cur->input = open(line, O_RDONLY);
            cur->change = 1;
            in_flag = 1;
         }
         else if(in_flag == 1 || line == NULL)
         {
            printf("cmd: bad input redirection\n");
            /*exit(-1);*/
            return NULL;
         }
         
      }
      else if(strcmp(line, ">") == 0) /* OUTPUT */
      {
         if(out_flag == 0) /* VALID OUTPUT */
         {
            strcat(cur->name, " > ");
            line = strtok(NULL, " ");

            strcat(cur->name, line);
            cur->output = open(line, O_WRONLY | O_TRUNC | O_CREAT, 0666);
            cur->change = 1;
            out_flag = 1;
         }
         else if(out_flag == 1 || line == NULL)
         {
            printf("cmd: bad output redirection\n");
            /*exit(-1);*/
            return NULL;
         }
      }
      else if(strcmp(line, "|") == 0) /* PIPE */
      {
         if(out_flag == 1)
         {
            printf("Ambiguous output\n");
            /* exit(-1); */
            return NULL;
         }
         line = strtok(NULL, " ");
         if(strcmp(line, "|") == 0)
         {
            printf("Invalid null command.\n");
            /*exit(-1);*/
            return NULL;
         }

         if((stage + 1) > 9)
         {
            printf("Pipeline too deep.\n");
            /* exit(-1) */
            return NULL;
         }

         cmd_arr[stage] = cur; 
         stage++;
         cur = create_command(line, stage);
      }
      else if(line != NULL) /* ARGV */ 
      {
         
         if((cur->argc) == 9)
         {
            printf("Too many arguments.\n");
            /*exit(-1);*/
            return NULL;
         }
         strcat(cur->name, " ");
         strcat(cur->name, line);
         strcpy(cur->args[cur->argc], line);
         cur->argc++;
      }
   }
   
   /*for(i = 0; i < 10; i++)
   {
      if(cmd_arr[i] != NULL)
      {
         print_cmd(cmd_arr[i]);
      }
   }
   free(cmd_arr);*/
   return cmd_arr;
}
