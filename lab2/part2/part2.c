#include "part2.h"
#include <ctype.h>
#include <string.h>


char* str_lower(char string[], char buffer[]){
	int i;

	for (i=0; i < strlen(string); i++){
		*(buffer+i) = tolower(*(string+i));
	}
	return buffer;
}

void str_lower_mutate(char string[]){
	int i;
	for (i=0; i < strlen(string) ; i++){
		*(string+i) = tolower(string[i]);
	}
}