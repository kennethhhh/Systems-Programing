#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct command{
	char name[128];
	char input[128];
	char output[128];
	char argv[64][64];
	int argc;
	int stage;
};

struct command * create_command(char *, int);
void print_cmd(struct command *);
void free_cmds(struct command **);