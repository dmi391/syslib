/*
 * interrupt.h
 *
 *      Author: PehotinDO
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "clint.h"
#include "plic.h"


//Interrupt common

extern "C" void SetHandler(void (*ptrHandler)(void));
extern "C" void SetMepc(void (*ptrReturn)(void));
extern "C" void handler(void) __attribute__((interrupt, aligned(4)));


////////////////

///Убрать!!!

extern inline void __attribute__((always_inline))  get_interrupt_diag(void);

extern inline void __attribute__((always_inline)) void_interrupt_compleat(void);

void interrupt_compleat(void);


void EnableExternInterrupt(void);
void DisableExternInterrupt(void);

/**
 * Диагностика при возникновении прерывания/исключения (глобально в секции .diag_section)
 */
typedef struct
{
	unsigned long int mcause;
	unsigned long int mepc;
	unsigned long int mtval;
	unsigned long int mie;
	unsigned long int mip;

}_interrupt_diag;

//==================================================


#endif /* INTERRUPT_H_ */
