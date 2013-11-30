# include <stdio.h>
# include <syscall.h>
# include <libc/malloc.h>
# include <sys/v_mem_manager.h>
# include <sys/task_management.h>

/*
* User space malloc which calls p_malloc() in kernel to allocate no_bytes 
* Argument to p_malloc() will always be in page multiple 
* malloc will manage rest of the memory here and return from local if it has some memory left
*/

char *pt_mal = NULL;
uint64_t size_left = 0;

void *malloc(uint64_t no_bytes)
{
	char *tmp = NULL; 
	uint32_t div = 0;
	uint32_t nu = 0, final = 0;

	if (!no_bytes)
	{
		return NULL;
	}

	if (pt_mal && (no_bytes < size_left))	// when malloc already have no_bytes available locallly, NO need to do brk() syscall
	{
		tmp = pt_mal;
		size_left -= no_bytes;
		pt_mal += no_bytes;

		return ((void *) tmp);		// return from local
	}

	/* else calculate and make a  syscall */
	div = (no_bytes / 4096);
	nu = (no_bytes % 4096);

	if (div)
	{
		final = (div * 4096);
	}
	
	if (nu)
	{
		final += 4096;
	}

	//call p_malloc(final);
	
	if ((tmp = (char*) __syscall1(SYSCALL_MALLOC,final)) == NULL)
	{
	//	printf("\n Cant allocalte user space memory");
		return NULL;
	}

	size_left = (final - no_bytes);
	
	pt_mal = (tmp + no_bytes);

	return ((void *) tmp);
}	


