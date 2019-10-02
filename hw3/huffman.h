#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct node{
	int count;
	unsigned char letter;
	struct node *next;
	struct node *left;
	struct node *right;
};

struct linked_list{
	struct node *head;
	int length;
};

struct node create_node(char ch, int c);
struct linked_list create_ll();
void reinsert(struct linked_list *l, struct node *n);
struct node * pop(struct linked_list *l);
void build_tree(struct linked_list *l);
int * count_frequency(int file, int *arr);
void link_the_nodes(struct linked_list *l, int *freq);
int is_leaf(struct node n);
void get_codes(struct node n, char *code, char **arr_codes, int last);
void print_code(char **codes);
int count_unique(int *table);
int header(int infile, int outfile, struct linked_list *l);
int rebuild_LL(int infile, struct linked_list *l);