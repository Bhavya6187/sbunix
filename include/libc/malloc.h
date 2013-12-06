#ifndef _MALLOC_H
#define _MALLOC_H

# include <defs.h>

void *malloc(uint64_t no_bytes);	// User space malloc implementation
uint64_t fork();			// User space fork wrapper
void execve(char *filename);
void execvp(char *filename, char **argv);
void yield();
void exit(int status);
uint64_t getpid();
uint64_t waitpid(uint64_t pid);
uint64_t wait();
void sleep(uint64_t time);
#endif
