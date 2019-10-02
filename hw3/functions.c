#include "huffman.h"

/* htable functions */
struct node create_node(char ch, int c){
	struct node n;
	n.count = c;
	n.letter = ch;
	n.next = NULL;
	n.left = NULL;
	n.right = NULL;
	return n;
}

struct linked_list create_ll(){
	struct linked_list ll;
	ll.head = NULL;
	ll.length = 0;
	return ll;
}

void reinsert(struct linked_list *l, struct node *n){
	struct node *current;
	struct node *newnode = (struct node *)malloc(sizeof(struct node));
	newnode -> count = (*n).count;
	newnode -> letter = (*n).letter;
	newnode -> left = (*n).left;
	newnode -> right = (*n).right;

	if(l -> head == NULL){
		l -> head = newnode;
		(*l).length++;
		return;
	}

	/* INSERTING TO LINKED LIST */
	if((*n).letter){
		(*l).length++;
		if(l -> head -> count > (*n).count){
			newnode -> next = l -> head;
			l -> head = newnode;
			return;
		}

		else if(l -> head -> count == (*n).count){
			for(current = l -> head; current -> next != NULL && current -> next -> count == (*n).count && (*n).letter > (current -> next -> letter); current = current -> next);
			newnode -> next = current -> next;
			current -> next = newnode;
			return;
		}

		else{
			for(current = l -> head; current -> next != NULL && current -> next -> count < (*n).count; current = current -> next);
			for(current = current; current -> next != NULL && current -> next -> count == (*n).count && (*n).letter > (current -> next -> letter); current = current -> next);
			newnode -> next = current -> next;
			current -> next = newnode;
		}
	}

	/* CREATING TREE */
	else{
		(*l).length++;
		if(l -> head -> count >= (*n).count){
			newnode -> next = l -> head;
			l -> head = newnode;
			return;
		}

		for(current = l -> head; current -> next != NULL && current -> next -> count < (*n).count; current = current -> next);
		newnode -> next = current -> next;
		current -> next = newnode;
	}
}

struct node * pop(struct linked_list *l){
	struct node *front = l -> head;
	l -> head = l -> head -> next;
	(*l).length--;
	return front;
}
void build_tree(struct linked_list *l){
	struct node *first_node = pop(l);
	struct node *second_node = pop(l);
	struct node *combined = (struct node *)malloc(sizeof(struct node));


	combined -> left = first_node;
	combined -> right = second_node;
	combined -> count = ((first_node -> count) + (second_node -> count));
	reinsert(l, combined);
}

int * count_frequency(int file, int *arr){
	unsigned char ch;

	while ((read(file, &ch, sizeof(char))) != 0){
		arr[(int)ch]++;
	}
	return arr;
}

void link_the_nodes(struct linked_list *l, int *freq){
	int i;
	struct node n;

	for(i = 0; i < 256; i++){
		if (freq[i] != 0){
			n = create_node((char)i, freq[i]);
			reinsert(l, &n);
		}
	}
}

int is_leaf(struct node n){
	if((n.right)==NULL && (n.left)==NULL){
		return 1;
	}
	return 0;
}

void get_codes(struct node n, char *code, char **arr_codes, int last){
	if(is_leaf(n)){
		arr_codes[(int)n.letter] = (char *)calloc(1, sizeof(code));
		/*strncpy(destination, thing you wanna copy, how many chars do you wanna copy in that 2nd param)*/
		strncpy(arr_codes[(int)n.letter], code, last);
	}
	if(n.left){
		code[last] = '0';
		get_codes(*(n.left), code, arr_codes, last + 1);
	}
	if(n.right){
		code[last] = '1';
		get_codes(*(n.right), code, arr_codes, last + 1);
	}
}

void print_code(char **codes){
	int i;
	for(i = 0; i < 256; i++){
		if(codes[i]){
			printf("0x%x: %s\n",i, codes[i]);
		}
	}
}

/*HENCODE FUNCTIONS*/
int count_unique(int *table){
	int i;
	int total = 0;
	for(i = 0; i < 256; i++){
		if(table[i]!=0){
			total++;
		}
	}
	return total;
}

int header(int infile, int outfile, struct linked_list *l){
	int *frequency_table = (int *)calloc(256, sizeof(int));
	int header_size, i;

	frequency_table = count_frequency(infile, frequency_table);
	header_size = count_unique(frequency_table);

	if (header_size==0){
		return 1;
	}

	link_the_nodes(l, frequency_table);
	while((*l).length != 1){
		build_tree(l);
	}

	write(outfile, &header_size, sizeof(int));
	for(i = 0; i < 256; i++){
		if(frequency_table[i] != 0){
			write(outfile, &i, sizeof(char));
			write(outfile, &(frequency_table[i]), sizeof(int));
		}
	}
	return 0;
}


/* HDECODE FUNCTIONS */
int rebuild_LL(int infile, struct linked_list *l){
	int *header_size = (int *)calloc(256, sizeof(int));
	int *count = (int *)calloc(256, sizeof(int));
	int i;
	int total = 0;
	char *letter = (char *)calloc(1, sizeof(char));
	struct node n;
	
	read(infile, header_size, sizeof(int));

	for(i =0; i < *header_size; i++){
		read(infile, letter, sizeof(char));
		read(infile, count, sizeof(int));
		total += *count;
		n = create_node(*letter, *count);
		reinsert(l, &n);
	}

	return total;
}
