/*
 * syslib.h
 *
 * 		Author: PehotinDO
 */

#ifndef SYSLIB_SYSLIB_H_
#define SYSLIB_SYSLIB_H_

#include <cstdint>

//==================================================
//CSRs for interrupts from PLIC

#define CSR_MIE_MEIE			0x0800	//csr mie.meie
#define CSR_MSTATUS_MIE			0x0008	//csr mstatus.mie

extern "C" void EnableExternalInterrupt();
extern "C" void DisableExternalInterrupt();


//PLIC fields

#define PLIC_PRIORITY			*(volatile uint32_t*)0x0C000000 //1: 0x0C000004; 2: 0x0C000008; 3: 0x0C000008
	#define PLIC_PRIORITY_1		*(volatile uint32_t*)0x0C000004
	#define PLIC_PRIORITY_2		*(volatile uint32_t*)0x0C000008
	#define PLIC_PRIORITY_3		*(volatile uint32_t*)0x0C00000C
#define PLIC_ENABLE				*(volatile uint32_t*)0x0C002000
#define PLIC_THRESHOLD			*(volatile uint32_t*)0x0C200000
#define PLIC_PENDING			*(volatile uint32_t*)0x0C001000
#define PLIC_CLAIM				*(volatile uint32_t*)0x0C200004

enum state
{
	ENABLE = true,
	DISABLE = false
};

extern "C" void SetPlicPriority(uint8_t id, uint8_t priority);
extern "C" void SetPlicEnable(uint8_t id, state st);
extern "C" void SetPlicThreshold(uint8_t threshold);
extern "C" uint32_t GetPlicPending(void);
extern "C" uint32_t PlicComplete(void);
extern "C" void PlicClearPending(void);

//==================================================




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
    register unsigned long readval; \
    asm volatile ("csrr %[in], " #reg :[in] "=r"(readval) ::); \
    readval; })


#define WriteCsr(reg, val) ({ \
    asm volatile ("csrw "#reg", %[out]" ::[out] "r"(val):); })


#define SetBitsCsr(reg, bits) ({ \
	asm volatile ("csrs "#reg", %[out]" ::[out] "r"(bits):); })


#define ClearBitsCsr(reg, bits) ({ \
	asm volatile ("csrc "#reg", %[out]" ::[out] "r"(bits):); })


#endif /* SYSLIB_SYSLIB_H_ */
