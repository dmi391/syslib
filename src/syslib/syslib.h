/*
 * syslib.h
 *
 * Author: PehotinDO
 */

#ifndef SYSLIB_SYSLIB_H_
#define SYSLIB_SYSLIB_H_


extern "C" void Enable_ExternalInterrupt(void);
extern "C" void Disable_ExternalInterrupt(void);
extern "C" void SetHandler(void (*ptrHandler)(void));
extern "C" void SetMepc(void (*ptrReturn)(void));




extern "C" void handler(void) __attribute__((interrupt, aligned(4))); //__attribute__ ((aligned (8))) //__attribute__((section(".interrupt_handler"), interrupt));
extern "C" void enother_handler(void) __attribute__((interrupt, aligned(4)));




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

#define ReadCsr(reg) ({ \
    register unsigned long __tmp; \
    asm volatile ("csrr %[in], " #reg :[in] "=r"(__tmp) ::); \
    __tmp; })


#define WriteCsr(reg, val) ({ \
    asm volatile ("csrw "#reg", %[out]" ::[out] "r"(val):); })


#define SetBitsCsr(reg, bits) ({ \
	asm volatile ("csrs "#reg", %[out]" ::[out] "r"(bits):); })


#define ClearBitsCsr(reg, bits) ({ \
	asm volatile ("csrc "#reg", %[out]" ::[out] "r"(bits):); })


#endif /* SYSLIB_SYSLIB_H_ */
