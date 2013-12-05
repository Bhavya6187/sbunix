#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <libc/dir.h>
#include <stdarg.h>
#include<defs.h>

int main(int argc, char* argv[]) 
{

  int i=1;
  i++;
  u_printf("In world.c with i = %d\n",i);
  
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
