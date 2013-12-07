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
	uint64_t ppid;	// Process ID	uint64_t ppid;	// Process ID
	uint64_t pml4e_entry;	// Entry of process in pml4e table
	uint64_t* u_stack;	// Process ID
	struct pcb_t *prev;
	struct pcb_t *next;
	VMA *mm_st;	// Pointer to First VMA Block of the Process 
	uint64_t cr3;	// Coontents of CR3 Register. (add of PML4E page table)
	uint64_t cow;	// Value of COW bit will tell how many processes with current process as parent are present 
	uint64_t start_time;
  char name[64];
	uint64_t sleep_start;	
	uint64_t sleep_duration;	
	uint64_t rip;
  /*
  uint64_t rax;
  uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
  */
	uint64_t rsp;	
	/*
  uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
  */
	uint64_t kernel_stack[256];	//abhi check for the size
};

typedef struct pcb_t PCB;


// Creating a Doubly Linked List
struct taskList {
  struct taskList *next;
  struct taskList *prev;
  struct pcb_t *task;
};

struct taskList *addToHeadTaskList(struct taskList *list, struct pcb_t *new_pcb);
struct taskList *addToTailTaskList(struct taskList *list, struct pcb_t *new_pcb);
struct taskList *removeFromTaskList(struct taskList *list, struct pcb_t *rem_pcb);
struct taskList *moveTaskToEndOfList(struct taskList *list);

uint64_t get_curr_PID();
uint64_t get_pid();	// Returns a free PID from the pid bitmap
uint64_t get_PID_PCB(struct pcb_t * gpcb);

PCB *get_parent_PCB(uint64_t parent_pid);
PCB *searchPCB(struct taskList *tlist, uint64_t pid);
PCB *get_curr_PCB();

PCB *create_pcb();	// DO initial Setup on a new process creation
VMA *create_vma(uint64_t start, uint64_t size);	// Creates VMA structure for each segment 

uint64_t doFork();
void doExec(char* filename);
void doExecvp(char *fn, char **);

void exit_process(int status);
uint64_t wait_pid(uint64_t pid);
uint64_t wait_p();
void sleep_t(uint64_t time);
void checkAwake();

extern struct taskList *waitTaskQ;
extern struct taskList *allTaskQ;
extern struct taskList *runnableTaskQ;
extern PCB * running;
extern int no_waitQ, no_allQ, no_runnableQ;
void scheduler1();

void PS_Display();

void init_process();
uint64_t kill_process(int pid);
void clear_screen();

#endif
