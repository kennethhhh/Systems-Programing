#include "part3.h"

int sum(int stuff[], int length){
	int total;
	int i;

	if (length == 0){
		return 0;
	}
	
	for (i=0; i<length; i=i+1){
		total = total + stuff[i];
	}
	return total;
}
