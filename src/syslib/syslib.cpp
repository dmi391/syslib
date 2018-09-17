/*
 * syslib.cpp
 *
 *      Author: PehotinDO
 */

#include "syslib.h"

_interrupt_diag interrupt_diag __attribute__((section (".diag_section"))) __attribute__((aligned(8))) = {0};


//Функция формирования диагностики при возникновении прерывания/исключения
void get_interrupt_diag(void)
{
	clean_interrupt_diag(); //Обнуление структуры interrupt_diag

	//Запись в глобальную структуру interrupt_diag
	asm volatile(
						"csrr %[r_mcause], mcause \n\t"
						"csrr %[r_mtval], 0x343 \n\t" //mtval (раньше mbadaddr)
						"csrr %[r_mepc], mepc \n\t"
						"csrr %[r_mie], mie \n\t"
						"csrr %[r_mip], mip"
						: [r_mcause] "=r" (interrupt_diag.mcause),
						  [r_mtval] "=r" (interrupt_diag.mtval),
						  [r_mepc] "=r" (interrupt_diag.mepc),
						  [r_mie] "=r" (interrupt_diag.mie),
						  [r_mip] "=r" (interrupt_diag.mip)
						::
		    	);
	return;
}


extern "C"
{
	//Обнуление диагностики прерываний
	void clean_interrupt_diag(void)
	{
		unsigned long int *ptr = (unsigned long int*)&interrupt_diag;
		for(int i = 0; i < sizeof(interrupt_diag)/sizeof(unsigned long int); i++)
		{
			ptr[i] = 0;
		}
		return;
	}
}


//Инициализация подсистем машины перед началом работы
extern "C"
{
	void init(void)
	{
		//...
		return;
	}
}
