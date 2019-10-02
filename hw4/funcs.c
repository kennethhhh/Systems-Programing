#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include "dirent.h"
#include "funcs.h"
#include "time.h"
#include <arpa/inet.h>
#include <stdint.h>

/*
ASK CHRIS

IF CHKSUM FUNCTION IS CORRECT
IF HEADER->UID / GID (UID TOO LONG)
*/

#define F_FLAG 1
#define C_FLAG 2
#define V_FLAG 4
#define T_FLAG 8
#define X_FLAG 16
#define S_FLAG 32
#define SIZE 256

void usage() {
  perror("Usage: mytar [ctxv]f tarfile [path [...] ]\n");
  exit(-1);
}

void create_header_file(int tfile, char *pathname, struct stat *buf, struct ustar_header *header){
   mode_t m = buf->st_mode;
   struct passwd *usr;
   struct group *grp;

   create_name_prefix(pathname, header);

   sprintf(header->mode, "%07ho", (unsigned short) (buf->st_mode & 0x1ff)); /* write mode */

   insert_special_int(header->uid, 7, buf->st_uid);

   sprintf(header->gid, "%07o", buf->st_gid); /* write gid */

   if(S_ISREG(m)){ /* write size and typeflag */
      sprintf(header->size, "%011lo", (long) buf->st_size); /* write size of regular file */
      header->typeflag = '0'; /* set regular file typeflag */
   }
   else{
      sprintf(header->size, "%011lo", (unsigned long)0); /* write 0 if sym link or dir */
      if(S_ISDIR(m))
         header->typeflag = '5'; /* set dir typeflag */
      else
         header->typeflag = '2'; /* set sym link typeflag */
   }

   sprintf(header->mtime, "%lo", (long) buf->st_mtime); /* write mtime */

   strncpy(header->chksum, "        ", sizeof(char) * 8); /* CHECKSUM */

   if(S_ISLNK(buf->st_mode)){ /* LINK NAME HERE */
      readlink(pathname, header->linkname, sizeof(char) * 100);
   }

   strncpy(header->magic, "ustar", sizeof(char) * 6); /* write magic */

   header->version[0] = header->version[1] = '0'; /* write version */

   usr = getpwuid(buf->st_uid); /* write uname */
   strcpy(header->uname, usr->pw_name);

   grp = getgrgid(buf->st_gid); /* write gname */
   strcpy(header->gname, grp->gr_name);
   get_chksum(header);
}

void create_name_prefix(char *pathname, struct ustar_header *header){
   int i = 0, len, j = 0;
   char ret_str[155] = {'\0'}, c, path[256];

   strcpy(path, pathname);
   len = strlen(path);
   if(len < 100)
   {
      while(i < 100){
         if(i < len)
            c = path[i];
         else{
            c = '\0';
         }
         header->name[i] = c;
         /*write(tfile, &c, sizeof(char));*/
         i++;
      }
      return;
   }
   i = len - 100;
   while(i < len && path[i] != '/')
   {
      i++;
   }
   strncpy(ret_str, path, i);
   i++;
   while(i < len){
      header->name[j] = path[i];
      i++;
      j++;
   }
   strcpy(header->prefix, ret_str);
}

void copy_contents(int tfile, int infile){
   char *buf = (char *)calloc(512, sizeof(char));
   int bytes_read;

   while((bytes_read = read(infile, buf, 512 * sizeof(char))) != 0)
   {
      write(tfile, buf, 512 * (sizeof(char)));
      free(buf);
      buf = (char *)calloc(512, sizeof(char));
   }
   free(buf);
}

void clear_header(struct ustar_header *header){
   clear_string(header->name, 100);
   clear_string(header->mode, 8);
   clear_string(header->uid, 8);
   clear_string(header->gid, 8);
   clear_string(header->size, 12);
   clear_string(header->mtime, 12);
   clear_string(header->chksum, 8);
   header->typeflag = '\0';
   clear_string(header->linkname, 100);
   clear_string(header->magic, 6);
   clear_string(header->version, 2);
   clear_string(header->uname, 32);
   clear_string(header->gname, 32);
   clear_string(header->devmajor, 8);
   clear_string(header->devminor, 8);
   clear_string(header->prefix, 155);
   clear_string(header->padding, 12);
}

void clear_string(char *string, int length){
   int i;
   for(i = 0; i < length; i++)
      string[i] = '\0';
}

void get_chksum(struct ustar_header *header){
   int sum = 0, i;

   for(i = 0; i < 100; i++)
   {
     sum += (unsigned char)header->name[i];
   }
   for(i = 0; i < 8; i++)
   {
     sum += (unsigned char)header->mode[i];
   }
   for(i = 0; i < 8; i++)
   {
     sum += (unsigned char)header->uid[i];
   }
   for(i = 0; i < 8; i++)
   {
     sum += (unsigned char)header->gid[i];
   }
   for(i = 0; i < 12; i++)
   {
     sum += (unsigned char)header->size[i];
   }
   for(i = 0; i < 12; i++)
   {
     sum += (unsigned char)header->mtime[i];
   }
   for(i = 0; i < 8; i++)
   {
     sum += (unsigned char)header->chksum[i];
   }
   for(i = 0; i < 100; i++)
   {
     sum += (unsigned char)header->linkname[i];
   }
   for(i = 0; i < 6; i++)
   {
     sum += (unsigned char)header->magic[i];
   }
   for(i = 0; i < 2; i++)
   {
     sum += (unsigned char)header->version[i];
   }
   for(i = 0; i < 32; i++)
   {
     sum += (unsigned char)header->uname[i];
   }
   for(i = 0; i < 32; i++)
   {
     sum += (unsigned char)header->gname[i];
   }
   for(i = 0; i < 155; i++)
   {
     sum += (unsigned char)header->prefix[i];
   }

   sum += (unsigned char)header->typeflag;
   clear_string(header->chksum, 8);
   sprintf(header->chksum, "%07o", sum);
}

void write_header(int tfile, struct ustar_header *header){
   write(tfile, header->name, sizeof(char) * 100);
   write(tfile, header->mode, sizeof(char) * 8);
   write(tfile, header->uid, sizeof(char) * 8);
   write(tfile, header->gid, sizeof(char) * 8);
   write(tfile, header->size, sizeof(char) * 12);
   write(tfile, header->mtime, sizeof(char) * 12);
   write(tfile, header->chksum, sizeof(char) * 8);
   write(tfile, &header->typeflag, sizeof(char));
   write(tfile, header->linkname, sizeof(char) * 100);
   write(tfile, header->magic, sizeof(char) * 6);
   write(tfile, header->version, sizeof(char) * 2);
   write(tfile, header->uname, sizeof(char) * 32);
   write(tfile, header->gname, sizeof(char) * 32);
   write(tfile, header->devmajor, sizeof(char) * 8); /* write major number: null */
   write(tfile, header->devminor, sizeof(char) * 8); /* write minor number: null */
   write(tfile, header->prefix, sizeof(char) * 155); /* write prefix */
   write(tfile, header->padding, sizeof(char) * 12);/* write padding */
}

int check_flags_fcvtxs(char *arg)
{
  int i;
  int flags = 0;
  for(i = 0; i < strlen(arg); i++)
  {
    if(arg[i] == 'f')
    {
      flags |= F_FLAG;
    }
    else if(arg[i] == 'c')
    {
      flags |= C_FLAG;
    }
    else if(arg[i] == 'v')
    {
      flags |= V_FLAG;
    }
    else if(arg[i] == 't')
    {
      flags |= T_FLAG;
    }
    else if(arg[i] == 'x')
    {
      flags |= X_FLAG;
    }
    else if(arg[i] == 's')
    {
      flags |= S_FLAG;
    }
  }

  return flags;
}

/* this is a helper function to help dirwalk recurse through subdirectories */
void recurse_dir(char *dir, int tfile,  struct ustar_header *header,int flags, void(*fcn)(char *, int, struct ustar_header*, int))
{
  char name[SIZE];
  struct dirent *dp;
  DIR *dfd;

  if((dfd = opendir(dir)) == NULL)
  {
    perror("dirwalk: can't open file");
    return;
  }
  while((dp = readdir(dfd)) != NULL)
  {
    if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
    {
      continue;
    }
    if(strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
    {
      fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
    }
    else
    {
      sprintf(name, "%s%s", dir, dp->d_name);
      (*fcn)(name, tfile, header, flags);
    }
  }
  closedir(dfd);
}

/* dirwalk takes in the name of a directory and recurses through until it
reaches each subdirectory / file / symbolic link in the root directory */
void dirwalk(char *name, int tfile, struct ustar_header *header, int flags)
{
  struct stat stbuf;
  int infile;

  if(lstat(name, &stbuf) == -1)
  {
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if(S_ISDIR(stbuf.st_mode)) /* FOUND A DIRECTORY */
  {
    strcat(name, "/");
    if(flags & V_FLAG){
      printf("%s\n", name);
    }
    clear_header(header);
    create_header_file(tfile, name, &stbuf, header);
    write_header(tfile, header);
    recurse_dir(name, tfile, header, flags, dirwalk);
  }
  else if(S_ISREG(stbuf.st_mode) || S_ISLNK(stbuf.st_mode)) /*FILE OR SYMBOLIC LINK */
  {
    /* CALL TAR FUNCTIONS ON FILES HERE */
    infile = open(name, O_RDONLY);
    if(infile < 0){
      printf("Could not open file: %s\n", name);
    } else {
      if((flags & V_FLAG)){
        printf("%s\n", name);
      }
      clear_header(header);
      create_header_file(tfile, name, &stbuf, header);
      write_header(tfile, header);
      copy_contents(tfile, infile); /* only for file, sym link */
      close(infile);
    }
  }
}

void pad_file(int tfile){
   char pad[512] = {'\0'};
   write(tfile, pad, 512);
   write(tfile, pad, 512);
}

uint32_t extract_special_int(char *where, int len)
{
  int32_t val = -1;
  if((len >= sizeof(val)) && (where[0] & 0x80))
  {
    val = *(int32_t *)(where + len - sizeof(val));
    val = ntohl(val);
  }
  return val;
}

int insert_special_int(char *where, size_t size, int32_t val)
{
  int err = 0;
  if(val < 0 || (size < sizeof(val))){
    err++;
  } else{
    memset(where, 0, size);
    *(int32_t *)(where + size - sizeof(val)) = htonl(val);
    *where |= 0x80;
  }

  return err;
}

char * permissions(char ch){
  switch(ch){
    case '0': return "---";
    case '1': return "--x";
    case '2': return "-w-";
    case '3': return "-wx";
    case '4': return "r--";
    case '5': return "r-x";
    case '6': return "rw-";
    case '7': return "rwx";
  }
}

int YUH(char *shring){
  int i;
  char ustar[6] = "ustar ";
  for(i = 0; i < 6; i++){
    /*printf("ustar:%c\tstring:%c\n", ustar[i], shring[i]);*/
    if (ustar[i] != shring[i]){
      /*printf("WRONG... ustar:%c (%d)\tstring:%c (%d)\n", ustar[i], ustar[i], shring[i], shring[i]);
      printf("-1\n");*/
      return -1;
    }
  }
  /*printf("1\n");*/
  return 1;
}

void make_table(int tfile, int flags){
   char path[SIZE] = {'\0'}, mode[11] = {'\0'}, ugname[18] = {'\0'}, mtime[17] = {'\0'};
   char *tmp = (char *)calloc(101, sizeof(char));
   int size, i, filesize;
   struct ustar_header header;
   time_t t;

   if(tfile < 0){
      exit(-1);
   }

   while((size = read(tfile, &header, sizeof(char) * 512)) != 0){
      clear_string(tmp, 101);
      if(strcmp("ustar", header.magic) == 0 || YUH(header.magic) == 1){
         if(strlen(header.prefix) != 0){
            strcpy(path, header.prefix);
            strcat(path, "/");
         }
         strncpy(tmp, header.name, 100 *sizeof(char));
         header.name[strlen(header.name)] = '\0';
         strcat(path, tmp);

         if(flags & V_FLAG){
            filesize = strtol(header.size, NULL, 8);
            strcat(mode, (filesize == 0) ? "d" : "-");
            strcat(mode, (permissions(header.mode[4])));
            strcat(mode, (permissions(header.mode[5])));
            strcat(mode, (permissions(header.mode[6])));

            strcat(ugname, header.uname);
            strcat(ugname, "/");
            strcat(ugname, header.gname);
            i = strlen(ugname);
            while(i < 17){
               ugname[i] = ' ';
               i++;
            }

            t = strtol(header.mtime, NULL, 8);
            strftime(mtime, 17, "%Y-%m-%d %H:%M", localtime(&t));
            mtime[16] = '\0';

            printf("%s %s%9d %s ", mode, ugname, filesize, mtime);
         }

         if(header.prefix[0] != 0)
            printf("%s/", header.prefix);
         printf("%.100s\n", header.name);
         clear_string(path, SIZE);
         clear_string(mode, 11);
         clear_string(ugname, 18);
         clear_string(mtime, 17);
      }
   }
   free(tmp);
 }

 void make_table2(int tfile, int flags, char *pathname){
   char path[SIZE] = {'\0'}, mode[11] = {'\0'}, ugname[18] = {'\0'}, mtime[17] = {'\0'};
   char *tmp = (char *)calloc(101, sizeof(char));
   int size, i, filesize;
   struct ustar_header header;
   struct stat buffer;
   time_t t;

   if(tfile < 0){
      exit(-1);
   }

   while((size = read(tfile, &header, sizeof(char) * 512)) != 0){
      clear_string(tmp, 101);
      if(strcmp("ustar", header.magic) == 0 || YUH(header.magic) == 1){
         if(strlen(header.prefix) != 0){
            strcpy(path, header.prefix);
            strcat(path, "/");
         }
         strncpy(tmp, header.name, 100 *sizeof(char));
         header.name[strlen(header.name)] = '\0';
         strcat(path, tmp);

         if(strstr(path, pathname)!=NULL){
          if(flags & V_FLAG){
            filesize = strtol(header.size, NULL, 8);
            strcat(mode, (filesize == 0) ? "d" : "-");
            strcat(mode, (permissions(header.mode[4])));
            strcat(mode, (permissions(header.mode[5])));
            strcat(mode, (permissions(header.mode[6])));

            strcat(ugname, header.uname);
            strcat(ugname, "/");
            strcat(ugname, header.gname);
            i = strlen(ugname);
            while(i < 17){
               ugname[i] = ' ';
               i++;
            }

            t = strtol(header.mtime, NULL, 8);
            strftime(mtime, 17, "%Y-%m-%d %H:%M", localtime(&t));
            mtime[16] = '\0';

            printf("%s %s%8d %s ", mode, ugname, buffer.st_size, mtime);
          }

          if(header.prefix[0] != 0)
            printf("%s/", header.prefix);
            printf("%.100s\n", header.name);
         }
         
          clear_string(path, SIZE);
          clear_string(mode, 11);
          clear_string(ugname, 18);
          clear_string(mtime, 17);
      }
   }
   free(tmp);
 }


int count_slash(char *str){
  int i, total=0;
  for(i = 0; i < strlen(str); i++){
    if(str[i]=='/'){
      total++;
    }
  }
  return total;
}

char * parse(char *name, int count){
  int i;
  int index = 0;
  char *path = (char *)calloc(100, sizeof(char));

  for(i = 0; i < strlen(name); i++){
      if(count == 0){
        if(name[i] == '/'){
              return path;
        }
        
        path[index] = name[i];
        index++;
        if(i == (strlen(name)-1)){
            return path;
        }
      }
    else if(name[i] == '/'){
      count--;
    }
  }
}

void extract_contents(int tfile, int tmp_file, int buf_size, int unpad_bytes){
   int i, bytes_read;
   char tmp[512] = {'\0'};
   struct ustar_header header;
   for(i = 0; i < buf_size; i++){
     bytes_read = read(tfile, &header, sizeof(char) * 512);
     write(tmp_file, &header, sizeof(char) * 512);
   }
   
   if((bytes_read = read(tfile, &header, sizeof(char) * 512)) != 0){
     write(tmp_file, &header, sizeof(char) * unpad_bytes);
   }
 }

 void extraction(int tfile, int flags, char *argv[], int num_args)
 {
    char path[SIZE] = {'\0'}, tmp_name[101] = {'\0'}, placeholder[256] = {'\0'};
   struct ustar_header header;
   int fmode, fsize, bytes_read, tmp_file, buf_size = 0, unpad_bytes, i, slash, j;
   char *single;

   while((bytes_read = read(tfile, &header, sizeof(char) * 512)) != 0){
     if(strcmp("ustar", header.magic) == 0){
      fmode = strtol(header.mode, NULL, 8);
      fsize = strtol(header.size, NULL, 8);

      if(strlen(header.prefix) != 0){
        strcpy(path, header.prefix);
        strcat(path, "/");
      }

      strncpy(tmp_name, header.name, 100);
      strcat(path,tmp_name);
      if(num_args > 3){
        for(i = 3; i < num_args; i++){
          if(strstr(path, argv[i])!=NULL){
            slash = count_slash(path);
            single = parse(argv[i], slash);
            if(flags & V_FLAG){
              printf("%s\n", path);
            }
            if(header.typeflag == '5'){
              mkdir(path, fmode);
            }
            else if(header.typeflag == '2' || header.typeflag == '0'){
              for(j = 0; j < slash; j++){
                strcat(placeholder, parse(argv[i], j));
                mkdir(placeholder, fmode);
                strcat(placeholder, "/");
              }
              tmp_file = open(single, O_CREAT | O_WRONLY | O_TRUNC, fmode);
              unpad_bytes = fsize % 512;
              printf("%d\n", unpad_bytes);
              buf_size = fsize / 512; /*round up to nearest int */
              extract_contents(tfile, tmp_file, buf_size, unpad_bytes);
              close(tmp_file);
            }
            utime(path, header.mtime);
          }
        }
      }
      else{
        if(flags & V_FLAG){
         printf("%s\n", path);
        }
        if(header.typeflag == '5'){
         mkdir(path, fmode);
        }
        else if(header.typeflag == '2' || header.typeflag == '0'){
         tmp_file = open(path, O_CREAT | O_WRONLY | O_TRUNC, fmode);
         unpad_bytes = fsize % 512;
         printf("%d\n", unpad_bytes);
         buf_size = fsize / 512; /*round up to nearest int */
         extract_contents(tfile, tmp_file, buf_size, unpad_bytes);
         close(tmp_file);
        }
        utime(path, header.mtime);
      }
     }
     clear_string(path, SIZE);
   }
   close(tfile);
 }
