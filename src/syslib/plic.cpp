/*
 * plic.cpp
 *
 *      Author: PehotinDO
 */

#include "syslib.h"

//==================================================
//CSRs configuration

#define CSR_MIE_MEIE			0x0800	//csr mie.meie
#define CSR_MSTATUS_MIE			0x0008	//csr mstatus.mie

void EnableExternalInterrupt()	//For interrupts from PLIC
{
	asm volatile ("csrs		mie,		%[val]" ::[val] "r"(CSR_MIE_MEIE):);	//mie.meie = 1
	asm volatile ("csrs		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 1
}

void DisableExternalInterrupt()	//For interrupts from PLIC
{
	asm volatile("csrc		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 0
	asm volatile("csrc		mie,		%[val]" ::[val] "r"(CSR_MIE_MEIE):);	//mie.meie = 0
}

//==================================================
//PLIC configuration

///Убрать!!!
#define PLIC_PRIORITY_1_ADDR	0x0C000004
#define PLIC_PRIORITY_2_ADDR	0x0C000008
#define PLIC_PRIORITY_3_ADDR	0x0C00000C
#define PLIC_ENABLE_ADDR		0x0C002000
#define PLIC_THRESHOLD_ADDR		0x0C200000
#define PLIC_PENDING_ADDR		0x0C001000
#define PLIC_CLAIM_ADDR			0x0C200004


#define PLIC_PRIORITY			*(volatile uint32_t*)0x0C000000 //1: 0x0C000004; 2: 0x0C000008; 3: 0x0C000008
#define PLIC_PRIORITY_1			*(volatile uint32_t*)0x0C000004
#define PLIC_PRIORITY_2			*(volatile uint32_t*)0x0C000008
#define PLIC_PRIORITY_3			*(volatile uint32_t*)0x0C00000C
#define PLIC_ENABLE				*(volatile uint32_t*)0x0C002000
#define PLIC_THRESHOLD			*(volatile uint32_t*)0x0C200000
#define PLIC_PENDING			*(volatile uint32_t*)0x0C001000
#define PLIC_CLAIM				*(volatile uint32_t*)0x0C200004

enum state
{
	ENABLE = true,
	DISABLE = false
};


void PlicPriority(uint8_t id, uint8_t priority)
{
	(&PLIC_PRIORITY)[id] = priority;
}

void PlicEnable(uint8_t id, state st)
{

}

void PlicThreshold(uint8_t id, uint8_t threshold)
{

}

uint8_t PlicPending(void)
{

}

uint8_t PlicComplete(void)
{

}


void test()
{
	unsigned int* ip = (unsigned int*)0x0C200004;
	ip[1] = 0x1;

	*(volatile unsigned int*)0x0C200004 = 0x4;
	(&(*(unsigned int*)0x0C200004))[1] = 0x4;

	//p/x *(int)0x0c200004
}

///#define REG (* ( ( volatile int * ) 0x0C200004 ) );
///REG = 4;


////////////////


void EnableExternInterrupt()
{
	unsigned long int address = 0;
	unsigned long int value 	 = 0;

	//priority interrupt 1 = 7
	address = PLIC_PRIORITY_3_ADDR;
	value 	= 0x7;	//highest priority 0x7
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	address = PLIC_ENABLE_ADDR;	// enable interrupts
	//value 	= 0x2;			//enable only interrupt 1
	value 	= 0x8;
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	address = PLIC_THRESHOLD_ADDR;
	value 	= 0x0;				// thershold = 0
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	//enable extern interrupt in mie
	address = CSR_MIE_MEIE;
	asm volatile(
	"csrrs 	%[val], mie, 	zero\n\t"
	"or 	%[val], %[addr],%[val]\n\t"
	"csrrs 	zero, 	mie, 	%[val]"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	//enable interrupts mstatus.mie = 1
 	address = CSR_MSTATUS_MIE;
	asm volatile(
	"csrrs 	%[val], mstatus, zero\n\t"
	"or 	%[val], %[addr], %[val]\n\t"
	"csrrs 	zero, 	mstatus, %[val]"::
	[val] "rK"(value), [addr] "rK"(address):
	);
	return;
}

void DisableExternInterrupt()
{
	unsigned long int address = 0;
	unsigned long int value 	 = 0;

	address = CSR_MSTATUS_MIE;
	//disable interrupts mstatus.mie
	asm volatile(
	"csrrs 	%[val], mstatus, zero\n\t"
	"and    %[val], %[addr], %[val]\n\t"
	"csrrc 	zero, 	mstatus, %[val]"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	//disable extern interrupt in mie
	address = CSR_MIE_MEIE;
	asm volatile(
	"csrrs 	%[val], mie, 	zero\n\t"
	"and 	%[val], %[addr],%[val]\n\t"
	"csrrc 	zero, 	mie, 	%[val]"::
	[val] "rK"(value), [addr] "rK"(address):
	);
	return;
}

void ptr_interrupt_compleat(void)
{
	//long int *c = (long int*)PLIC_CLAIM_ADDR;
	unsigned long int claim = *(unsigned long int*)PLIC_CLAIM_ADDR;
	*(unsigned long int*)PLIC_CLAIM_ADDR = claim;
	return;
}




/**
 * @brief Функция опроса и записи claim для завершения обработки прерывания
 *
 */
extern inline void __attribute__((always_inline)) void_interrupt_compleat(void)
{
	unsigned long int address = PLIC_CLAIM_ADDR;
	unsigned long int value   = 0;
	asm volatile(
	"lw %[val], 0x0(%[addr])\n\t"
	"sw %[val], 0x0(%[addr])\n\t"
	::
	[val] "rK"(value), [addr] "rK"(address):
	);
	return;
}

void interrupt_compleat(void)
{
	unsigned int address = PLIC_CLAIM_ADDR;
	unsigned int value   = 0;
	asm volatile(
	"lw %[val], 0x0(%[addr])\n\t"
	"sw %[val], 0x0(%[addr])\n\t"
	::
	[val] "rK"(value), [addr] "rK"(address):
	);
	asm volatile("csrrs t0, mip, x0");///
	return;
}

//==================================================
