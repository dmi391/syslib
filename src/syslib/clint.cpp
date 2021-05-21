/*
 * clint.cpp
 *
 *      Author: PehotinDO
 */

#include "clint.h"

//==================================================
//CSRs configuration for CLINT

/**
 * @brief Enable timer interrupt from CLINT
 */
void EnableClintTimerInterrupt()
{
	asm volatile ("csrs		mie,		%[val]" ::[val] "r"(CSR_MIE_MTIE):);	//mie.mtie = 1
	asm volatile ("csrs		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 1
}

/**
 * @brief Disable timer interrupt from CLINT
 */
void DisableClintTimerInterrupt()
{
	asm volatile("csrc		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 0
	asm volatile("csrc		mie,		%[val]" ::[val] "r"(CSR_MIE_MTIE):);	//mie.mtie = 0
}

/**
 * @brief Enable software interrupt from CLINT
 */
void EnableClintSoftwareInterrupt()
{
	asm volatile ("csrs		mie,		%[val]" ::[val] "r"(CSR_MIE_MSIE):);	//mie.msie = 1
	asm volatile ("csrs		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 1
}

/**
 * @brief Disable software interrupt from CLINT
 */
void DisableClintSoftwareInterrupt()
{
	asm volatile("csrc		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 0
	asm volatile("csrc		mie,		%[val]" ::[val] "r"(CSR_MIE_MSIE):);	//mie.msie = 0
}

//==================================================
//CLINT configuration

/**
 * @brief Set clint timer
 */
void SetClintTimer(uint64_t period)
{
	CLINT_MTIMECMP = period;
	CLINT_MTIME = 0;
}

/**
 * @brief Set software interrupt from CLINT
 */
void SetClintSoftwareInterrupt()
{
	CLINT_MSIP = 1;
}

/**
 * @brief Clear software interrupt from CLINT
 */
void ClearClintSoftwareInterrupt()
{
	CLINT_MSIP = 0;
}
