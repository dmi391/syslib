/*
 * interrupt.cpp
 *
 *      Author: PehotinDO
 */

#include "../syslib.h"
#include "interrupt.h"


/**
 * @brief Handler for 0st source external interrupt
 * Never execute
 */
void HandlerExtern0(){}

/**
 * @brief Handler for 1st source external interrupt
 */
void HandlerExtern1()
{
	return;
}

/**
 * @brief Handler for 2nd source external interrupt
 */
void HandlerExtern2()
{
	return;
}

/**
 * @brief Handler for 3rd source external interrupt
 */
void HandlerExtern3()
{
	return;
}

/**
 * @brief Array of handlers external interrupts
 */
static void((*ArrayHandlers[4]))() {HandlerExtern0, HandlerExtern1, HandlerExtern2, HandlerExtern3};

/**
 * @brief Common interrupt and exception handler
 */
void handler(void)
{
	switch(ReadCsr(mcause))
	{
		case 0x8000000000000003:	//Machine software interrupt (CLINT)
		{
			ClearClintSoftwareInterrupt();
			DisableClintSoftwareInterrupt();
			break;
		}
		case 0x8000000000000007:	//Machine internal timer interrupt (CLINT)
		{
			DisableClintTimerInterrupt();
			break;
		}
		case 0x800000000000000b:	//Machine external interrupt (PLIC)
		{
			register uint32_t InterruptId = PlicComplete();	//0x3 = button
			//DisableExternalInterrupt();
			ArrayHandlers[InterruptId]();
			break;
		}
		default:	//Exceptions
		{
			break;
		}
	}
	return;	//mret
}

/**
 * @brief Set handler address to SCR mtvec
 */
void SetHandler(void (*ptrHandler)(void))	//For all interrupts/exceptions
{
	asm volatile ("csrw		mtvec,		%[tmp]" ::[tmp] "r"(ptrHandler):);
}

/**
 * @brief Set address (to CSR mepc) for return from handler
 */
void SetMepc(void (*ptrReturn)(void))	//For all interrupts/exceptions
{
	asm volatile ("csrw		mepc,		%[tmp]" ::[tmp] "r"(ptrReturn):);
}
