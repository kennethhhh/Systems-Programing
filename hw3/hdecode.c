#include "huffman.h"

int main(int argc, char const *argv[])
{
	int infile, outfile;
	struct linked_list l = create_ll();
	int ch, leaf, position, num_of_chars;
	struct node n;

	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0600);

	num_of_chars = rebuild_LL(infile, &l);

	if(l.length == 1){
		while(num_of_chars != 0){
			write(outfile, &(l.head -> letter), sizeof(char));
			num_of_chars--;
		}
	}

	if(l.length == 0){
		close(infile);
		close(outfile);
		return 0;
	}

	while(l.length != 1){
		build_tree(&l);
	}

	n = *(l.head);

	printf("%d\n", num_of_chars);
	while(num_of_chars > 0){
		read(infile, &ch, sizeof(char));
		for(position = 0; position < 8; position++){
			if(ch & 128){
				n = *(n.right);
			}
			if(!(ch & 128)){
				n = *(n.left);
			}
			leaf = is_leaf(n);
			if(leaf == 1){
				write(outfile, &(n.letter), sizeof(char));
				n = *(l.head);
				num_of_chars--;
				if(num_of_chars <= 0){
					break;
				}
			}
			ch = ch << 1;
		}

	}

	close(infile);
	close(outfile);

	return 0;
}