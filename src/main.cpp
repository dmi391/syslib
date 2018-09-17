#include "syslib/syslib.h"


int  main()
{

	//Пример переназначения обработчика прерываний
	set_mtvec((void(*)(void))&enother_handler);
	set_mtvec((void(*)(void))&handler);

	//Пример переназначения адреса возврата epc
	set_mepc((void(*)(void))&enother_handler);

	return 0;
}
