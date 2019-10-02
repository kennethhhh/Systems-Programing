#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* This function fills an empty array of integers with 0's (table)
   and based on the chars of the second parameter, finds the index
   corresponding to the char ascii value and puts a 1. The 1 
   signifies that the char of the index value needs to be deleted.*/
void delete_table(int table[], char set1[]){
   int i, ascii;

   for (i=0; i< 128; i++){
      table[i]=0;
   }

   for (i = 0; i < strlen(set1); i++){
      if (set1[i] == '\\'){
         i++;
         if (set1[i] == 'n'){
            ascii = '\n';
            table[ascii] = 1;
         }
         else if (set1[i] == 't'){
            ascii = '\t';
            table[ascii] = 1;
         }
         else{
            ascii = set1[i];
            table[ascii] = 1;
         }
      }
      else{
         ascii = set1[i];
         table[ascii] = 1;
      }
   }
}

/* Because escape characters are treated as 2 chars, // and a letter,
   reformat changes it so that is just 1 char. Normal characters are
   left alone. The first parameter is the given string and the 
   second parameter is the reformatted string. */
void reformat(char set1[], char t_set[]){
   int i, j=0;

   for (i=0; i < strlen(set1); i++){
      if (set1[i] == '\\'){
         i++;
         if (set1[i] == 'n'){
            t_set[j] = '\n';
         }
         else if (set1[i] == 't'){
            t_set[j] = '\t';
         }
         else{
            t_set[j] = set1[i];
         }
         j++;
      }
      else{
         t_set[j] = set1[i];
         j++;
      }
   }
}

/* This function takes the two given sets from the command line and 
   writes to an empty array of integers. This function will call 
   reformat on both given sets. The empty array is filled at the index
   of each char in the first formatted string with the corresponding
   value of char in the second formatted string. For example,
   'abc' 'def' --> table[(int)'a'] = (int) 'd' etc.*/
void translate_table (int table[], char set1[], char set2[]){
   int i, j, ascii;
   char *t_set1 = (char*)malloc((strlen(set1))*(sizeof(char)));
   char *t_set2 = (char*)malloc((strlen(set2))*(sizeof(char)));

   reformat(set1, t_set1);
   reformat(set2, t_set2);


   for (i=0; i< 128; i++){
      table[i]=i;
   }

   for (j=0; j < strlen(t_set1); j++){
      ascii = t_set1[j];
      if (j < strlen(t_set2)){
         table[ascii] = t_set2[j];
      }
      else{
         table[ascii] = t_set2[strlen(t_set2)-1];
      }
   } 
   free(t_set1);
   free(t_set2);
}

/* main determines if the number of arguments is correct
   and then determines if -d is the 2nd argument. If it is 
   -d, then it takes an infile and deletes the given chars
   (specified as the 3rd argument) and outputs to a file. 
   If the 2nd argument is a string, replace the chars in 
   first set with the second (chars correspond with each 
   index) for a given file and output to a file. */
int main(int argc, char *argv[]){
   int ch, translated, tmp;
   int table[128];

   if (argc == 3){
      if (strlen(argv[1]) == 2 && argv[1][0] == '-'){
         if (argv[1][1] == 'd'){
            delete_table(table, argv[2]);

            while((ch = getchar()) != EOF){
               if (table[ch] == 1){
                  continue;
               }
               else{
                  putchar(ch);
               }
            }
         }
         else{
            printf("Usage: mytr [-d] set1 set2\n");
         }
         
      }

      else{
         translate_table(table, argv[1], argv[2]);
      
         while ((ch = getchar()) != EOF){

            translated = table[(int)ch];
            putchar(translated);
         }
      }
   }

   else{
      printf("Usage: mytr [-d] set1 set2\n");
   }
   
   return 0;
}
