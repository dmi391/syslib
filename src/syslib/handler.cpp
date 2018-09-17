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

//Вызов Си-функции из ассемблера:
//При этом прототип функции должен быть следующим:
//extern "C" int Foo(int z);
//Определение:
//extern "C"
//{
//	int	Foo(int z)
//	{
//		return z;
//	}
//}
//вызов из ассемблера:
//call Foo
//вызов из Си:
//Foo(5);


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
