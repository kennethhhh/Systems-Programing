#include "huffman.h"

int main(int argc, char const *argv[])
{	
	int infile = open(argv[1], O_RDONLY);
	int outfile = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0600);
	int temp, i, is_empty;
	int position = 0;
	int byte = 0;
	char **array_of_codes = (char **)calloc(256, sizeof(char *));
	char *str = (char *)calloc(256, sizeof(char));
	char *code;
	unsigned char ch;
	char character_byte = 0;
	struct linked_list l = create_ll();

	/* WRITE HEADER */
	is_empty = header(infile, outfile, &l);
	if(is_empty == 1){
		write(outfile, &character_byte, sizeof(char));
		write(outfile, &character_byte, sizeof(char));
		write(outfile, &character_byte, sizeof(char));
		write(outfile, &character_byte, sizeof(char));
		close(infile);
		close(outfile);
		return 0;
	}


	get_codes(*(l.head), str, array_of_codes, 0);

	close(infile);
	temp = open(argv[1], O_RDONLY);

	/* WRITE BODY */
	while((byte = read(temp, &ch, sizeof(char))) != 0){
		code = array_of_codes[(int)ch];

		for(i = 0; i < strlen(code); i++){
			if(*(code+i) == '1'){
				character_byte |= 1;
			}
			
			position++;

			if(position == 8){
				write(outfile, &character_byte, sizeof(char));
				position = 0;
				character_byte = 0;
			}
			character_byte = character_byte << 1;
		}
	}

	if(character_byte != 0){
		while(position != 7){
			character_byte = character_byte << 1;
			position++;
		}
		write(outfile, &character_byte, sizeof(char));
	}
	close(temp);
	close(outfile);

	return 0;
}