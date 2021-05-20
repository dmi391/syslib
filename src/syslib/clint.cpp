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
 * @brief Clear CSR bits mip.msip and mip.mtip
 * Without handler.
 * mie.meie and/or mstatus.mie mast be closed.//????????????????????????????????
 */
void ClintClearMipMsipMtip(void)
{

}

