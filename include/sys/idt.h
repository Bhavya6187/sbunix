#ifndef _GDT_H
#define _GDT_H

#include <defs.h>

/* adapted from Chris Stones, shovelos */

#define IST           (0x0000)  /*** code segment descriptor ***/
#define RESERVED      (0x0000)  /*** data segment descriptor ***/
#define TYPE          (0x0E00)  /*** conforming ***/
#define ZERO          (0x0000)  /*** conforming ***/
#define DPL0          (0x0000)  /*** descriptor privilege level 0 ***/
#define DPL1          (0x2000)  /*** descriptor privilege level 1 ***/
#define DPL2          (0x4000)  /*** descriptor privilege level 2 ***/
#define DPL3          (0x6000)  /*** descriptor privilege level 3 ***/
#define P             (0x8000)  /*** present ***/

extern uint64_t gdt[];

void reload_gdt();

#endif
