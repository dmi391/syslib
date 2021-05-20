/*
 * syslib.h
 *
 * 		Author: PehotinDO
 */

#ifndef SYSLIB_SYSLIB_H_
#define SYSLIB_SYSLIB_H_

#include <cstdint>
//#include "interrupt_common.h"
#include "plic.h"
#include "clint.h"


//==================================================
//Interrupt common

extern "C" void SetHandler(void (*ptrHandler)(void));
extern "C" void SetMepc(void (*ptrReturn)(void));
extern "C" void handler(void) __attribute__((interrupt, aligned(4)));




//==================================================
//CSRs

#define ReadCsr(reg) ({ \
    register unsigned long readval; \
    asm volatile ("csrr %[in], " #reg :[in] "=r"(readval) ::); \
    readval; })


#define WriteCsr(reg, val) ({ \
    asm volatile ("csrw "#reg", %[out]" ::[out] "r"(val):); })


#define SetBitsCsr(reg, bits) ({ \
	asm volatile ("csrs "#reg", %[out]" ::[out] "r"(bits):); })


#define ClearBitsCsr(reg, bits) ({ \
	asm volatile ("csrc "#reg", %[out]" ::[out] "r"(bits):); })

//==================================================




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


#endif /* SYSLIB_SYSLIB_H_ */
