#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	/* code */
	int i;

	for (i=0 ; i < argc ; i++){
		if (strncmp(argv[i],"-",1)==0){
			printf("%s\n", argv[i]);
		}
	}
	return 0;
}