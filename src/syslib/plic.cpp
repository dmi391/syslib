/*
 * plic.cpp
 *
 *      Author: PehotinDO
 */

#include "syslib.h"

//==================================================
//CSRs configuration

/**
 * @brief Enable interrupts from PLIC
 */
void EnableExternalInterrupt()
{
	asm volatile ("csrs		mie,		%[val]" ::[val] "r"(CSR_MIE_MEIE):);	//mie.meie = 1
	asm volatile ("csrs		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 1
}

/**
 * @brief Disable interrupts from PLIC
 */
void DisableExternalInterrupt()
{
	asm volatile("csrc		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 0
	asm volatile("csrc		mie,		%[val]" ::[val] "r"(CSR_MIE_MEIE):);	//mie.meie = 0
}

//==================================================
//PLIC configuration

/**
 * @brief Set PLIC priority
 *
 * @param uint8_t id - Interrupt ID
 * @param uint8_t priority - Priority value
 */
void SetPlicPriority(uint8_t id, uint8_t priority)
{
	(&PLIC_PRIORITY)[id] = priority;
}

/**
 * @brief Set PLIC enable
 *
 * @param uint8_t id - Interrupt ID
 * @param state st - State (Enable or Disable)
 */
void SetPlicEnable(uint8_t id, state st)
{
	if(ENABLE == st)
	{
		PLIC_ENABLE |= (1 << id);
	}
	else //DISABLE
	{
		PLIC_ENABLE &= ~(1 << id);
	}
}

/**
 * @brief Set PLIC threshold
 *
 * @param uint8_t threshold - Threshold value
 */
void SetPlicThreshold(uint8_t threshold)
{
	PLIC_THRESHOLD = threshold;
}

/**
 * @brief Get PLIC pending
 *
 * @return PLIC pending
 */
uint32_t GetPlicPending(void)	//plic.pending is read only
{
	return PLIC_PENDING;
}

/**
 * @brief Read and complete PLIC interrupt
 * For one interrupt. For call from handler.
 *
 * @return Interrupt ID
 */
uint32_t PlicComplete(void)
{
	register uint32_t _claim = PLIC_CLAIM;
	PLIC_CLAIM = _claim;
	return _claim;
}

/**
 * @brief Clear plic.pending
 * Without handler.
 */
void PlicClearPending(void)
{
	//Save PLIC fields
	register uint32_t _plicPriority1 = (&PLIC_PRIORITY)[1];
	register uint32_t _plicPriority2 = (&PLIC_PRIORITY)[2];
	register uint32_t _plicPriority3 = (&PLIC_PRIORITY)[3];
	register uint32_t _plicEnable = PLIC_ENABLE;

	SetPlicPriority(1, 3);
	SetPlicPriority(2, 3);
	SetPlicPriority(3, 3);
	SetPlicEnable(1, ENABLE);
	SetPlicEnable(2, ENABLE);
	SetPlicEnable(3, ENABLE);

	//Clear all bits plic.pending
	register uint32_t claimVal = PLIC_CLAIM;
	while(claimVal != 0)
	{
		PLIC_CLAIM = claimVal;
		claimVal = PLIC_CLAIM;
	}

	//Restore PLIC fields
	(&PLIC_PRIORITY)[1] = _plicPriority1;
	(&PLIC_PRIORITY)[2] = _plicPriority2;
	(&PLIC_PRIORITY)[3] = _plicPriority3;
	PLIC_ENABLE = _plicEnable;
}

////////////////

///Убрать!!!
#define PLIC_PRIORITY_1_ADDR	0x0C000004
#define PLIC_PRIORITY_2_ADDR	0x0C000008
#define PLIC_PRIORITY_3_ADDR	0x0C00000C
#define PLIC_ENABLE_ADDR		0x0C002000
#define PLIC_THRESHOLD_ADDR		0x0C200000
#define PLIC_PENDING_ADDR		0x0C001000
#define PLIC_CLAIM_ADDR			0x0C200004


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
