#include <stdlib.h>
#include <stdio.h>
#include <syscall.h>
#include <libc/malloc.h>
#include <stdarg.h>
#include <defs.h>

//void _start( char** argv )
void _start( uint64_t argv )
{
	int argc = 1;
	//char* argv[10];
	char* envp[10];
  int res;
	res = main(argc, (char**)argv, envp);
	exit(res);
}
