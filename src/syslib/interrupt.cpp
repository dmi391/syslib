/*
 * interrupt.cpp
 *
 *      Author: PehotinDO
 */

#include "syslib.h"


/**
 * @brief Interrupt and exception handler
 */
extern "C"
{
	void handler(void)
 	{

		switch(ReadCsr(mcause))
		{
			case 0x8000000000000003:	//Machine software interrupt (CLINT)
			{

				break;
			}
			case 0x8000000000000007:	//Machine internal timer interrupt (CLINT)
			{

				break;
			}
			case 0x800000000000000b:	//Machine external interrupt (PLIC)
			{
				register uint32_t InterruptId = PlicComplete();	//0x3
				DisableExternalInterrupt();

				//array[claim]();
				break;
			}
			default:	//Exceptions
			{

				break;
			}
		}
		return;	//mret
	}
}


//Вызов по указателю:
//void((*array[]))() {0, handler1, handler2, handler3 };
//unsigned long int claim = interrupt_compleat();
//Вызов: array[claim](); //При таком вызове handler() сохраняет восстанавливает все регистры, а при использовании switch-case сохраняются и восстанавливаются только нужные


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

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




//EnableClintInterrupt()	//For interrupts from CLINT
//DisableClintInterrupt()	//For interrupts from CLINT
