#include "parseline.h"

struct command * create_command(char *cmd, int stage)
{
	struct command *tmp = (struct command *)calloc(1, sizeof(struct command));
	strcpy(tmp->name, cmd);
	strcpy(tmp->argv[0], cmd);
	strcpy(tmp->output, "original stdout");
	tmp->argc = 1;
	tmp->stage = stage;
	if(stage == 0)
	{
		strcpy(tmp->input, "original stdin");
	}
	else
	{
		sprintf(tmp->input, "pipe from stage %d", stage - 1);
	}

	return tmp;
}

void print_cmd(struct command *cmd)
{
	int i;
	printf("\n--------\n");
	printf("Stage %d: \" %s \"\n", cmd->stage, cmd->name);
	printf("--------\n");
	printf("     input: %s\n", cmd->input);
	printf("    output: %s\n", cmd->output);
	printf("      argc: %d\n", cmd->argc);
	printf("      argv: \"%s\"", cmd->argv[0]);
	for(i = 1; i < 64; i++)
	{
		if(strcmp(cmd->argv[i], "") != 0)
		{
			printf(",\"%s\"", cmd->argv[i]);
		}
		else
		{
			break;
		}
	}
	printf("\n");
}

void free_cmds(struct command **cmd_arr)
{
	int i;
	for(i = 0; i < 10; i++)
	{
		free(cmd_arr[i]);
	}
}
