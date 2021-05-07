/*
 * plic.cpp
 *
 *      Author: PehotinDO
 */

#include <cstdint>

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

#define PLIC_PRIORITY			0x0C00000C //0x0C000004, ...
#define PLIC_ENABLE				0x0C002000
#define PLIC_THRESHOLD			0x0C200000
#define PLIC_PENDING			0x0C001000
#define PLIC_CLAIM				0x0C200004


void PlicPriority()
{

}

void PlicEnable()
{

}


int* ip = (int*)0x0C200004;
///*((int*)0x0C200004) = 4;

///#define REG (* ( ( volatile int * ) 0x0C200004 ) );
///REG = 4;


////////////////


void EnableExternInterrupt()
{
	unsigned long int address = 0;
	unsigned long int value 	 = 0;

	//priority interrupt 1 = 7
	address = PLIC_PRIORITY;
	value 	= 0x7;	//highest priority 0x7
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	address = PLIC_ENABLE;	// enable interrupts
	//value 	= 0x2;			//enable only interrupt 1
	value 	= 0x8;
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	address = PLIC_THRESHOLD;
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
	//long int *c = (long int*)PLIC_CLAIM;
	unsigned long int claim = *(unsigned long int*)PLIC_CLAIM;
	*(unsigned long int*)PLIC_CLAIM = claim;
	return;
}




#define PLIC_CLAIM  0x0C200004
/**
 * @brief Функция опроса и записи claim для завершения обработки прерывания
 *
 */
extern inline void __attribute__((always_inline)) void_interrupt_compleat(void)
{
	unsigned long int address = PLIC_CLAIM;
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
	unsigned int address = PLIC_CLAIM;
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
