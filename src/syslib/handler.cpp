/*
 * handler.cpp
 *
 * Author: PehotinDO
 */

#include "syslib.h"

//Обработчик прерываний, назначаемый в startup
extern "C"
{
    void handler(void)
    {
    	get_interrupt_diag();
    	//...
    	return;
    }
}


//Для переназначения обработчика
extern "C"
{
	void enother_handler(void)
	{
		get_interrupt_diag();
		//...
		return;
	}
}
