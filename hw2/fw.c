#include "fw.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct word create_word(char *string){
	struct word w;
	w.string = (char *)calloc(strlen(string)+1, sizeof(char));
	strcpy(w.string, string);
	w.count = 1;
	return w;
}

struct hash_table create_ht(int resize, int num_items, int table_size){
	struct hash_table h;
	struct word *table = (struct word *)calloc(512*resize, sizeof(struct word));

	h.table = table;
	h.num_items = num_items;
	h.table_size = table_size;

	return h;
}

float load_factor(struct hash_table *h){
	return ((float)(*h).num_items/(float)(*h).table_size);
}

void ree(struct word w, struct hash_table *h){
	int i=0;
	int hash_val=0;
	hash_val = hash(w.string)%(*h).table_size;

	while ((*h).table[hash_val + i*i].string != NULL){
		i+=1;
		if (hash_val + i*i >= (*h).table_size){
			hash_val -= (*h).table_size;
		}
	}

	if (((*h).table[hash_val + i*i]).string == NULL){
		(*h).table[hash_val + i*i] = w;
	}
}

struct hash_table rehash(struct hash_table *h){
	int i, new_table_size, complicated;
	struct hash_table new_ht;
	struct word *array;

	new_table_size = (*h).table_size * 2;
	complicated = (*h).table_size / 128;
	new_ht = create_ht(complicated + 1, (*h).num_items, new_table_size);

	array = getwords(*h);
	for (i = 0; i < (*h).num_items; i++){
		ree(array[i], &new_ht);
	}
	return new_ht;
}

int hash(char *str){
	int i, total =0;
	for (i = 0; i < strlen(str); i++){
		total += (int)*(str+i);
	}
	return total;
}


void insert(char *str, struct hash_table *h){
	int i=0;
	int hash_val=0;
	struct word w;
	hash_val = hash(str)%(*h).table_size;

	while ((*h).table[hash_val + i*i].string && strcmp(((*h).table[hash_val + i*i]).string, str) != 0){
		i+=1;
		if (hash_val + i*i >= (*h).table_size){
			hash_val -= (*h).table_size;
		}
	}

	if (((*h).table[hash_val + i*i]).string == NULL){
		w = create_word(str);
		(*h).num_items ++;
		(*h).table[hash_val + i*i] = w;
	}
	
	else if (strcmp((*h).table[hash_val + i*i].string,str) == 0){
		(*h).table[hash_val + i*i].count ++;
	}

	if (load_factor(h)>0.5){
		*h = rehash(h);
	}
}


struct word * getwords(struct hash_table h){
	int i, j=0;
	int size = h.num_items;
	struct word *array = (struct word*)calloc(size, sizeof(struct word));
	for (i = 0; i < h.table_size; i++){
		if((h.table[i]).string != NULL){
			array[j] = h.table[i];
			j++;
		}
	}
	return array;
}

int cmpfunc(const void *a, const void *b){
	struct word w1 = *(struct word *)a;
	struct word w2 = *(struct word *)b;
	if ((w2.count - w1.count) == 0){
		return strcmp(w2.string, w1.string);
	}
	return w2.count - w1.count;
}

void print_hash(struct hash_table *h){
	int i;
	printf("%d\n", (*h).table_size );
	for (i = 0 ; i < (*h).table_size; i++){
		printf("INDEX: %d       WORD: %s      COUNT: %d\n", i, (*h).table[i].string, (*h).table[i].count);
	}
}

void words_to_ht(FILE *f, struct hash_table *ht){
	char ch;
	char *str1 = (char *)calloc(2, sizeof(char));
	char str2[2];

	while ((ch = getc(f)) != EOF){
		if (isalpha(ch) != 0){
			str1 = (char *)realloc(str1, (strlen(str1)+2)*sizeof(char));
			str2[0] = tolower(ch);
			str2[1] = '\0';
			strcat(str1,str2);
		}
		else if(str1[0] != '\0'){
			insert(str1, ht);
			str1 = (char *)realloc(str1, 2*sizeof(char));
			str1[0] = '\0';
		}
	}
	if(str1[0] != '\0'){
		insert(str1, ht);
	}	
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void print_array(int size, struct word *ptr){
	int i;
	for (i = 0; i < size; i++){
		printf("WORD: %s\tFREQ: %d\n", (*(ptr+i)).string, (*(ptr + i)).count );
	}
}

int main(int argc, char const *argv[]){
	struct word *onlywords;
	int i, times = 10, file_start = 1;
	FILE *f;
	struct hash_table ht = create_ht(1, 0, 128);

	if(argv[1][0] == '-'){
		if (argv[1][1] == 'n' && atoi(argv[2]) != 0){
			times = atoi(argv[2]);
			file_start = 3;
		}
		else{
			printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
		}
	}

	for(i = file_start; i < argc; i++){
		if(exists(argv[i]) == 0){
			printf("usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
			return 0;
		}
	}

	while (file_start < argc){
		f = fopen(argv[file_start], "r");
		words_to_ht(f,&ht);

		fclose(f);
		file_start++;
	}

	onlywords = getwords(ht);
	qsort(onlywords, ht.num_items, sizeof(struct word), cmpfunc);
	printf("The top %d words (out of %d) are:\n", times, ht.num_items);

	for(i = 0; i < times; i++){
		if(i < ht.num_items){
			printf("%9d %s\n", (*(onlywords+i)).count, (*(onlywords+i)).string);
		}	
	}
	
	return 0;
}