#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <libc/dir.h>
#include <stdarg.h>
#include<defs.h>
int main(int argc, char* argv[]) 
{

  u_printf("In world\n");
  stderr("\n\nJust tring out stderr\n\n");

  char* ch = "etc/abc.sh";
  int ret = check_shbang(ch);
  
  if(ret ==1)
  {
    u_printf("\nShbang found\n");
    
  }
  if(ret ==0)
    u_printf("\nShbang not found\n");
  
  char **params=NULL;
  for(int i=0;i<10;i++)
  {
    params[i] = (char*)malloc(sizeof(char)*10);
    params[i] = "\0";
  }
  
  int num = read_shbang(ch,params);
  execvp(params[0],&params[1]);

  for(int i=0;i<num;i++)
  {
    u_printf("%s",params[i]);
  }
/*  u_printf("Opening ch %s",ch);
  int fd = open(ch);
  if(fd==-1)
    u_printf("File not found");
  else
  {
    u_printf("\nFile has fd %d",fd);
    char st[20];
    int i = 0;

    for(i = 0; i<10;i++){
      read(fd,1,st);
        u_printf("char at %d - %c\n",i,st[0]);
    }
  }
   close(fd) ;

  struct dirent* dir = (struct dirent*)malloc(sizeof(struct dirent* ));
  DIR* directory = u_opendir("bin/");

  do{
  dir = u_readdir(directory);
  u_printf("%s \n\n",dir->d_name);
  }while(dir!=NULL);*/

//  while(1);
  
  /*
  struct dirent* dir;
  DIR* directory;

  directory = u_opendir("bin/");
  u_printf("\nin dir structure  = %s\n",directory->dirname);
  dir = u_readdir(directory);
  u_printf("\n in dirent structure = %s\n",dir->d_name);
  dir = u_readdir(directory);
  u_printf("\n in dirent structure = %s\n",dir->d_name);
  dir = u_readdir(directory);
  u_printf("\n in dirent structure = %s\n",dir->d_name);
  u_closedir(directory);
  */
  //while(1);
  
	return 0;
}
