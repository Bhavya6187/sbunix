#include <sys/idt.h>

/* adapted from Chris Stones, shovelos */

#define MAX_IDT 256

struct idt_entry {
  uint16_t offset1;
  uint16_t target_selector;
  uint16_t flags;
//  IST | RESERVED | TYPE | ZERO | DPL0| P,
  uint16_t offset2;
  uint32_t offset3;
  uint32_t reserved;
}__attribute__((packed));


struct idt_entry get_entry(uint64_t target)
{
   struct idt_entry ret;
   ret.offset1 = (target) & 0x0000ffff;
   ret.flags = IDT_IST | IDT_RESERVED | IDT_TYPE | IDT_ZERO | IDT_DPL0| IDT_P;
   ret.target_selector = 8;
   ret.reserved = 0;
   ret.offset2 = ((target) >> 16) & 0x0000ffff;
   ret.offset3 = ((target) >> 32) & 0xffffffff;
   return ret;
}


struct idt_entry idt[MAX_IDT];// = { get_entry((uint64_t)&_isr_000), get_entry((uint64_t)&_isr_001)};

struct idtr_t {
	uint16_t size;
	uint64_t addr;
}__attribute__((packed));

static struct idtr_t idtr = {
	sizeof(idt),
	(uint64_t)idt
};

extern void  _isr_000();

void _x86_64_asm_lidt(struct idtr_t* idtr);
void reload_idt() {
  
  idt[0] = get_entry((uint64_t)&_isr_000);
  //idt[32] = get_entry((uint64_t)&_irq_032);
 	_x86_64_asm_lidt(&idtr);
}


