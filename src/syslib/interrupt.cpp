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


#define CLAIM_COMPLETE_REG  0x0C200004
/**
 * @brief Функция опроса и записи claim для завершения обработки прерывания
 *
 */
extern inline void __attribute__((always_inline)) void_interrupt_compleat(void)
{
	unsigned long int address = CLAIM_COMPLETE_REG;
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
	unsigned int address = CLAIM_COMPLETE_REG;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
#define MIE_MEIE				0x0800
#define MSTATUS_MIE 			0x0008

#define PRIORITY_REG			0x0C000004
#define ENABLE_REG				0x0C002000
#define THRESHOLD_REG			0x0C200000
#define CLAIM_COMPLETE_REG		0x0C200004


void EnableExternInterrupt()
{
	unsigned long int address = 0;
	unsigned long int value 	 = 0;

	//priority interrupt 1 = 7
	address = PRIORITY_REG;
	value 	= 0x7;	//highest priority
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	address = ENABLE_REG;	// enable interrupts
	value 	= 0x2;			//enable only interrupt 1
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	address = THRESHOLD_REG;
	value 	= 0x0;				// thershold = 0
	asm volatile (
	"sw 	%[val], 0x0(%[addr])"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	//enable extern interrupt in mie
	address = MIE_MEIE;
	asm volatile(
	"csrrs 	%[val], mie, 	zero\n\t"
	"or 	%[val], %[addr],%[val]\n\t"
	"csrrs 	zero, 	mie, 	%[val]"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	//enable interrupts mstatus.mie = 1
 	address = MSTATUS_MIE;
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

	address = MSTATUS_MIE;
	//disable interrupts mstatus.mie
	asm volatile(
	"csrrs 	%[val], mstatus, zero\n\t"
	"and    %[val], %[addr], %[val]\n\t"
	"csrrc 	zero, 	mstatus, %[val]"::
	[val] "rK"(value), [addr] "rK"(address):
	);

	//disable extern interrupt in mie
	address = MIE_MEIE;
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
	//long int *c = (long int*)CLAIM_COMPLETE_REG;
	unsigned long int claim = *(unsigned long int*)CLAIM_COMPLETE_REG;
	*(unsigned long int*)CLAIM_COMPLETE_REG = claim;
	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


void Enable_ExternalInterrupt()
{
	asm volatile ("csrs		mie,		%[val]" ::[val] "r"(MIE_MEIE):);	//mie.meie = 1
	asm volatile ("csrs		mstatus,	%[val]" ::[val] "r"(MSTATUS_MIE):);	//mstatus.mie = 1
}


void Disable_ExternalInterrupt()
{
	asm volatile("csrc		mstatus,	%[val]" ::[val] "r"(MSTATUS_MIE):);	//mstatus.mie = 0
	asm volatile("csrc		mie,		%[val]" ::[val] "r"(MIE_MEIE):);	//mie.meie = 0
}


void SetHandler(void (*ptrHandler)(void))
{
	asm volatile ("csrw		mtvec,		%[tmp]" ::[tmp] "r"(ptrHandler):);
}


void SetMepc(void (*ptrReturn)(void))
{
	asm volatile ("csrw		mepc,		%[tmp]" ::[tmp] "r"(ptrReturn):);
}

