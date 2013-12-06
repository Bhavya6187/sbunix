#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <libc/dir.h>
#include <stdarg.h>
#include<defs.h>

int main(int argc, char* argv[]) 
{

  u_printf("In world\n");
  u_printf("%p\n", &argv);  
  u_printf("%s\n", argv[0]);  
  u_printf("%s\n", argv[1]);  
  while(1);
  int i=1;
  i++;
  u_printf("In world.c with i = %d\n",i);
  stderr("\n\nJust tring out stderr\n\n");
  char temp[6];
  u_scanf("%s",(uint64_t)temp);
  u_printf("\nscanf gave temp as = %s\n",temp);
  int tester;
  u_scanf("%d",(uint64_t)&tester);
  u_printf("\nscanf gave tester as = %d\n",tester);

  char* ch = "etc/abc.sh";
  u_printf("Opening ch %s",ch);
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

  while(1);
  
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
