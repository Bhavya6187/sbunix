#include <sys/idt.h>
#include <stdio.h>

void isr_handler_0()
{
     printf("Error: Divide by zero.. !! Grrr.... \n");
} 

