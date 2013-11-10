#ifndef _TASKING_H
#define _TASKING_H

struct pcb
{
	uint64_t rsp_p;		// rsp register for kernel stack
	uint64_t k_stack[64];	// kernel stack for the process
};

void call_first();
void schedule();

#endif

