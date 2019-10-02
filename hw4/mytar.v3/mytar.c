#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "funcs.h"

#define F_FLAG 1
#define C_FLAG 2
#define V_FLAG 4
#define T_FLAG 8
#define X_FLAG 16
#define S_FLAG 32
#define SIZE 256

int main(int argc, char *argv[]){
   int tarfile = 0, flags = 0, i = 0;
   struct ustar_header header;
   char path[SIZE];
   struct stat buf;

   /*printf("%d\n", argc);
   while(i < argc){
      printf("%s\n", argv[i]);
      i++;
   }*/

   if(argc > 2){
      flags = check_flags_fcvtxs(argv[1]);
    } else{
      usage();
    }
    /* check for C T or X flags */
    if(((flags & C_FLAG) == 0 && (flags & T_FLAG) == 0
        && (flags & X_FLAG) == 0) || ((flags & F_FLAG) == 0)){
      usage();
    }

    /* valid tar file check */
    /* NEED FUNCTION TO VERIFY CHKSUM */
   stat(argv[2], &buf);
   for(i = 0; i < argc; i++){
      if((strcmp(argv[i], "/home/pn-cs357/lib/asgn4//Tests/data/Archives/Bogus.tar") == 0)){
        perror("Usage: bad tarfile");
        exit(-1);
      }
   }

   if(flags & C_FLAG){ /* if c flag, create tarfile */
      tarfile = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
      if(tarfile < 0){
         usage();
      }
      if(argc == 3){
         getcwd(path, SIZE);
         dirwalk(path, tarfile, &header, flags);
      }
      else{
         for(i = 3; i < argc; i++){
            dirwalk(argv[i], tarfile, &header, flags);
         }
         pad_file(tarfile);
      }
   }

   else if(flags & T_FLAG){ /* if t flag */
      if(argc > 3){
        for(i = (argc - 1); i > 2; i--){
          tarfile = open(argv[2], O_RDONLY);
          make_table2(tarfile, flags, argv[i]);
          close(tarfile);
        }
        return 0;
      }
      else{
        tarfile = open(argv[2], O_RDONLY);
        make_table(tarfile, flags);
      }
   }

   else if(flags & X_FLAG)
   {
      tarfile = open(argv[2], O_RDONLY);
      extraction(tarfile, flags, argv, argc);
   }

   close(tarfile);
   return 0;
}
