# include <stdio.h>
# include <libc/malloc.h>
# include <sys/v_mem_manager.h>
# include <syscall.h>

void exit(int status) 
{
    __syscall1(SYSCALL_EXIT, status);
}
