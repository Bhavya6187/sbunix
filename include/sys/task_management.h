#ifndef _TASK_MANAGEMENT_H
#define _TASK_MANAGEMENT_H

#include <defs.h>
#define MAXPID 32	//32767	//

// VM Area Structure Definition
struct vma_struct
{
	struct mm_struct *vma_mm;      // associated mm_struct  // not sure what is this
	uint64_t  vma_start_add;  // VMA start inclusisve
	uint64_t vma_end_add;     // VMA end exclusive i.e. next adderess after VMA end
	struct vma_struct *vma_next;
	unsigned long vma_flags;   // flags
	 
};
typedef struct vma_struct VMA;

// Process Control Block 
struct pcb_t
{
	uint64_t pid;	// Process ID
	uint64_t ppid;	// Process ID
	struct pcb_t *prev;
	struct pcb_t *next;
	VMA *mm_st;	// Pointer to First VMA Block of the Process 
	uint64_t cr3;	// Coontents of CR3 Register. (add of PML4E page table)
	uint64_t rip;
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rsp;	
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t kernel_stack[64];	//abhi check for the size
};
typedef struct pcb_t PCB;

// Creating a Doubly Linked List
struct taskList {
  struct taskList *next;
  struct taskList *prev;
  struct pcb_t *task;
};

uint32_t get_pid();	// Returns a free PID from the pid bitmap
PCB *create_pcb();	// DO initial Setup on a new process creation
VMA *create_vma(uint64_t start, uint64_t size);	// Creates VMA structure for each segment 

extern struct taskList *waitTaskQ;
extern struct taskList *allTaskQ;
extern struct taskList *runnableTaskQ;

#endif
