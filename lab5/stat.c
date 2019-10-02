#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void file_type(struct stat buffer){
	if(S_ISREG(buffer.st_mode)){
		printf("Type:\tRegular File\n");
	}
	else if(S_ISDIR(buffer.st_mode)){
		printf("Type:\tDirrectory\n");
	}
	else if(S_ISCHR(buffer.st_mode)){
		printf("Type:\tCharacter Device\n");
	}
	else if(S_ISBLK(buffer.st_mode)){
		printf("Type:\tBlock Device\n");
	}
	else if(S_ISFIFO(buffer.st_mode)){
		printf("Type:\tNamed Pipe\n");
	}
	else if(S_ISLNK(buffer.st_mode)){
		printf("Type:\tSymbolic Link\n");
	}

}

int main(int argc, char const *argv[])
{	
	int i;
	struct stat buffer;

	for(i = 1; i < argc; i++){
		stat(argv[i], &buffer);

		printf("File:\t%s\n", argv[i]);
		file_type(buffer);
		printf("Size:\t%d\n", buffer.st_size);
		printf("Inode:\t%d\n", buffer.st_ino);
		printf("Links:\t%d\n", buffer.st_nlink);
		printf("Access:\t");
		printf( (S_ISDIR(buffer.st_mode)) ? "d" : "-");
	    printf( (buffer.st_mode & S_IRUSR) ? "r" : "-");
	    printf( (buffer.st_mode & S_IWUSR) ? "w" : "-");
	    printf( (buffer.st_mode & S_IXUSR) ? "x" : "-");
	    printf( (buffer.st_mode & S_IRGRP) ? "r" : "-");
	    printf( (buffer.st_mode & S_IWGRP) ? "w" : "-");
	    printf( (buffer.st_mode & S_IXGRP) ? "x" : "-");
	    printf( (buffer.st_mode & S_IROTH) ? "r" : "-");
	    printf( (buffer.st_mode & S_IWOTH) ? "w" : "-");
	    printf( (buffer.st_mode & S_IXOTH) ? "x" : "-");
	    printf("\n\n");
	}
	/* code */
	return 0;
}