void swap(int *pointer1, int *pointer2){
	int temp = *pointer1;
	*pointer1 = *pointer2;
	*pointer2 = temp;
}