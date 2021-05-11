#include "syslib/syslib.h"


int  main()
{

	//Пример использования gcc-asm
		unsigned long output = 0x0;
		unsigned long input = 0x33221100;
		asm volatile("csrrw %[out], mtval, %[in]"
					:[out] "=r"(output)
					:[in] "r"(input)
					://clobbers
					 );


	//Примеры использования csr-макросов
		int r0 = ReadCsr(mcycle);
		int r1 = ReadCsr(mcycle);

		WriteCsr(mtval, 0x30ff);		//0x30ff
		WriteCsr(mtval, 0x80abcd);		//0x80abcd
		SetBitsCsr(mtval, 0x1000ff);	//0x90abff
		ClearBitsCsr(mtval, 0x800055);	//0x10abaa


	//Пример переназначения обработчика прерываний
		//SetHandler((void(*)(void))&handler);
		//SetHandler(&enother_handler);

	//Пример переназначения адреса возврата mepc
		//SetMepc(&enother_handler);


///////////////////////////////////////////////
	PlicPriority(1, 2);
	PlicPriority(1, 7);//0x3
	PlicPriority(2, 2);
	PlicPriority(3, 3);
	PlicPriority(4, 4); //Не записывается. Остается 0x0




	SetHandler(&enother_handler);

	int i = 1;
	//asm(".word 0x80ffffff"); //exception: illegal instruction

	EnableExternInterrupt();
	i++;

	EnableExternInterrupt();
	i++;
	asm("wfi");
	i++;

	EnableExternInterrupt();
	i++;
	asm("wfi");
	i++;

	EnableExternInterrupt();
	i++;
	asm("wfi");
	i++;

	EnableExternInterrupt();
	i++;
	asm("wfi");
	i++;

	return 0;
}
