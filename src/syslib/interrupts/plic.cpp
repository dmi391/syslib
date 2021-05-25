/*
 * plic.cpp
 *
 *      Author: PehotinDO
 */

#include "plic.h"


//==================================================
//CSRs configuration for PLIC

/**
 * @brief Enable interrupts from PLIC
 */
void EnableExternalInterrupt(void)
{
	asm volatile ("csrs		mie,		%[val]" ::[val] "r"(CSR_MIE_MEIE):);	//mie.meie = 1
	asm volatile ("csrs		mstatus,	%[val]" ::[val] "r"(CSR_MSTATUS_MIE):);	//mstatus.mie = 1
}

/**
 * @brief Disable interrupts from PLIC
 */
void DisableExternalInterrupt(void)
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
 * mie.meie and/or mstatus.mie mast be closed.
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
