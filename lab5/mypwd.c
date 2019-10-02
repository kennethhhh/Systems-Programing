#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	DIR *current_dir;
	struct stat buffer, root;
	int inode;
	struct dirent *drip;
	char path[500];
	char next[500];

	stat(".", &buffer);
	inode = buffer.st_ino;

	while(chdir("..") != -1){
		stat(".", &root);
		if(root.st_ino == inode){
			break;
		}
		current_dir = opendir(".");
		strcpy(next,"/");

		while((drip = readdir(current_dir)) != NULL){
			printf("%d\n", drip);
			if (drip -> d_ino == inode){
				stat(drip -> d_name, &buffer);
				printf("%s\n", drip-> d_name);
				/*this creates a "/directory" */
				strcat(next, drip -> d_name);
				/*this creates a "/director/whatever_before" */
				strcat(next, path);
				strcpy(path, next);
				inode = buffer.st_ino;
				printf("%d\n", inode);
				break;
			}
		}
		closedir(current_dir);
	}

	printf("%s\n", path);
	return 0;
}