#ifndef _TASKING_H
#define _TASKING_H

struct pcb
{
	uint64_t rsp_p;		// rsp register for kernel stack
	uint64_t k_stack[64];	// kernel stack for the process
  uint64_t *u_stack;
  int pid;
  uint64_t cr3;
  struct mm_struct* mem;
};

struct mm_struct
{
  uint64_t size;
  uint64_t start;
};
void call_first();
void schedule();
void switch_to_user_mode();
#endif
