#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <defs.h>

#define SYSCALL_PROTO(n) static __inline uint64_t __syscall##n
#define SYSCALL_PUTINT 1
#define SYSCALL_PUTS 2 
#define SYSCALL_FORK 3 
#define SYSCALL_MALLOC 4
#define SYSCALL_SCANF 5
SYSCALL_PROTO(0)(uint64_t n) {
   uint64_t ret;
   __asm volatile( "movq %1,%%rax\n\t"
                   "int $80\n\t"
                   "movq %%rax,%0\n\t;"
                   :"=r"(ret):"r"(n):"rax","memory");
	return ret;
}

SYSCALL_PROTO(1)(uint64_t n, uint64_t a1) {
   uint64_t ret;
   __asm volatile( "movq %1,%%rax\n\t"
                   "movq %2,%%rbx\n\t"
                   "int $80\n\t"
                   "movq %%rax,%0\n\t;"
                   :"=r"(ret):"r"(n),"r"(a1):"rax","rbx","memory");
	return ret;
}

SYSCALL_PROTO(2)(uint64_t n, uint64_t a1, uint64_t a2) {
   uint64_t ret;
   __asm volatile( "movq %1,%%rax\n\t"
                   "movq %2,%%rbx\n\t"
                   "movq %3,%%rcx\n\t"
                   "int $80\n\t"
                   "movq %%rax,%0\n\t;"
                   :"=r"(ret):"r"(n),"r"(a1),"r"(a2):"rax","rbx","rcx","memory");
	return ret;
}

SYSCALL_PROTO(3)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	return 0;
}

SYSCALL_PROTO(4)(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) {
	return 0;
}

#endif
