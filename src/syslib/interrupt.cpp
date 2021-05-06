/*
 * interrupt.cpp
 *
 *      Author: PehotinDO
 */

#include "syslib.h"

_interrupt_diag interrupt_diag __attribute__((section (".diag_section"))) __attribute__((aligned(8))) = {0};


//Обработчик исключений/прерываний
extern "C"
{
	void handler(void)
	{
		DisableExternInterrupt();
		get_interrupt_diag();

		/*
		unsigned long int claim = interrupt_compleat();
		switch(claim)
		{
			case 1:
			{
				handler1();
				break;
			}
			case 2:
			{
				handler2();
				break;
			}
			case 3:
			{
				handler3();
				break;
			}
			default:
			{
				break;
			}
		}
		*/

		//array[claim]();
		return;
	}
}


//Другой обработчик исключений/прерываний
extern "C"
{
	void enother_handler(void)
 	{
		get_interrupt_diag();

		switch(interrupt_diag.mcause)
		{
			case 0x8000000000000003: //Machine software interrupt
			{

				break;
			}
			case 0x8000000000000007: //Machine internal timer interrupt
			{

				break;
			}
			case 0x800000000000000b: //Machine external interrupt
			{
				interrupt_compleat();
				DisableExternInterrupt();

				//array[claim]();
				break;
			}
			default:	//Всё остальное
			{

				break;
			}
		}
		return;
	}
}


//Вызов по указателю:
//void((*array[]))() {0, handler1, handler2, handler3 };
//unsigned long int claim = interrupt_compleat();
//Вызов: array[claim](); //При таком вызове handler() сохраняет восстанавливает все регистры, а при использовании switch-case сохраняются и восстанавливаются только нужные


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Функция формирования диагностики при возникновении прерывания/исключения
 *
 */
extern inline void __attribute__((always_inline)) get_interrupt_diag(void)
{
	//Обнуление структуры interrupt_diag
	///clean_interrupt_diag();

	//Запись в глобальную структуру interrupt_diag
	asm volatile(
						"csrr %[r_mcause], mcause \n\t"
						"csrr %[r_mtval], 0x343 \n\t" //mtval (раньше mbadaddr)
						"csrr %[r_mepc], mepc \n\t"
						"csrr %[r_mie], mie \n\t"
						"csrr %[r_mip], mip \n\t"
						: [r_mcause] "=r" (interrupt_diag.mcause),
						  [r_mtval] "=r" (interrupt_diag.mtval),
						  [r_mepc] "=r" (interrupt_diag.mepc),
						  [r_mie] "=r" (interrupt_diag.mie),
						  [r_mip] "=r" (interrupt_diag.mip)
						::
		    	);
	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void SetHandler(void (*ptrHandler)(void))	//For all interrupts/exceptions
{
	asm volatile ("csrw		mtvec,		%[tmp]" ::[tmp] "r"(ptrHandler):);
}


void SetMepc(void (*ptrReturn)(void))	//For all interrupts/exceptions
{
	asm volatile ("csrw		mepc,		%[tmp]" ::[tmp] "r"(ptrReturn):);
}




//EnableClintInterrupt()	//For interrupts from CLINT
//DisableClintInterrupt()	//For interrupts from CLINT
