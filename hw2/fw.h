#include <stdio.h>

struct word
{
	char *string;
	int count;
	int t_size;
};

struct hash_table
{
	struct word *table;
	int num_items;
	int table_size;
};

int horner_hash(char *string, struct hash_table *h);
float load_factor(struct hash_table *h);
int quad_probe_index(char *str, struct hash_table *h);
struct hash_table rehash(struct hash_table *h);
void ree(struct word w, struct hash_table *h);
void insert(char *str, struct hash_table *h);
struct word * getwords(struct hash_table h);
int cmpfunc(const void *a, const void *b);
void words_to_ht(FILE *f, struct hash_table *ht);
int *probe_index(char *str, struct hash_table *h);
int hash(char *str);
struct word create_word(char *string);
struct hash_table create_ht(int resize, int num_items, int table_size);
