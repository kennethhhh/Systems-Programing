#include "huffman.h"

int main(int argc, char const *argv[])
{
	int f;
	int *freq = (int *)calloc(256, sizeof(int));
	struct linked_list l = create_ll();
	struct node *curr;
	char **arraycodes = (char **)calloc(256, sizeof(char *));
	char *code = (char *)calloc(256, sizeof(char));

	f = open(argv[1], O_RDONLY);
	freq = count_frequency(f, freq);

	link_the_nodes(&l, freq);

	curr = l.head;
	printf("LL\n");
	while (curr != NULL){
		printf("Letter: %c\t Count: %d\n", curr -> letter, curr -> count);
		curr = curr -> next;
	}

	while(l.length != 1){
		build_tree(&l);
	}

	get_codes(*(l.head), code, arraycodes, 0);
	print_code(arraycodes);
	close(f);


	printf("Head ch: %c\t Head count: %d\n", l.head -> letter, l.head -> count);
	printf("Head left ch: %c\t Head left count: %d\n", l.head -> left-> letter, l.head -> left -> count);
	printf("Head right ch: %c\t Head right count: %d\n", l.head -> right-> letter, l.head -> right -> count);
	printf("Head left left ch: %c\t count: %d\n", l.head -> left-> left -> letter, l.head -> left-> left -> count);
	printf("Head left right ch: %c\t count: %d\n", l.head -> left->right-> letter, l.head ->left-> right -> count);
	printf("Head left left left ch: %c\t count: %d\n", l.head -> left-> left ->left-> letter, l.head -> left-> left ->left-> count);
	printf("Head left left right ch: %c\t count: %d\n", l.head -> left-> left ->right-> letter, l.head -> left-> left -> right->count);
	printf("Head right right ch: %c\t count: %d\n", l.head -> right-> right -> letter, l.head -> right -> right->count);
	printf("Head right left ch: %c\t count: %d\n", l.head -> right-> left->letter, l.head -> right -> left->count);

	return 0;

}