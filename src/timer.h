#ifndef _TIMER_H
#define _TIMER_H

#include <cpu.h>

#define CLINT_TIMER_CMP 0x02004000
#define CLINT_TIMER 0x0200bff8
#define IT_FREQ 20

void seance2(void);

extern void mon_traitant(void);
void trap_handler(uint32_t mcause, uint32_t mie, uint32_t mip);
uint32_t nbr_secondes(void);
void init_traitant_timer(void (*traitant)(void));
void enable_timer();

#endif