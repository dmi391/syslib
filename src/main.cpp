#include "syslib/syslib.h"
#include "timer.h"


int  main()
{
/*
	//Пример использования gcc-asm
		unsigned long output = 0x0;
		unsigned long input = 0x33221100;
		asm volatile("csrrw %[out], mtval, %[in]"
					:[out] "=r"(output)
					:[in] "r"(input)
					://clobbers
					 );


	//Примеры использования csr-макросов
		unsigned long r0 = ReadCsr(mcycle);
		unsigned long r1 = ReadCsr(mcycle);
		WriteCsr(mtval, 0x30ff);		//0x30ff
		WriteCsr(mtval, 0x80abcd);		//0x80abcd
		SetBitsCsr(mtval, 0x1000ff);	//0x90abff
		ClearBitsCsr(mtval, 0x800055);	//0x10abaa


	//Пример переназначения обработчика прерываний
		//SetHandler((void(*)(void))&handler);
		//SetHandler(&enother_handler);


	//Пример переназначения адреса возврата mepc
		//SetMepc(&enother_handler);


	//Примеры записи в plic.priority[]
	//(gdb) set p/x (&*(unsigned int*)0x0C000000)[1] = 0x3
	//(gdb) set (&PLIC_PRIORITY)[3] = 0x3
	SetPlicPriority(0, 2);	//(&0x0C000000)[0] = 0x0
	SetPlicPriority(1, 2);	//(&0x0C000000)[1] = &0x0C000004 = 0x2
	SetPlicPriority(1, 9);	//&0x0C000004 = 0x1 (WARL)
	SetPlicPriority(1, 7);	//&0x0C000004 = 0x3 (WARL)
	SetPlicPriority(2, 2);	//&0x0C000008 = 0x2
	SetPlicPriority(3, 3);	//&0x0C00000C = 0x3
	SetPlicPriority(4, 4);	//&0x0C000010 = 0x0

	//Примеры чтения plic.priority[]
	//(gdb) p/x (&*(unsigned int*)0x0C000000)[3]
	uint32_t priority = (&PLIC_PRIORITY)[3];


	//Примеры записи в plic.enable
	//(gdb) set *(unsigned int*)0x0C002000 = 6
	//(gdb) set PLIC_ENABLE = 6
	SetPlicEnable(1, ENABLE);	//0x2
	SetPlicEnable(4, ENABLE);	//0x2 (WARL)
	SetPlicEnable(3, ENABLE);	//0xa
	SetPlicEnable(1, DISABLE);	//0x8

	//Примеры чтения plic.enable
	//(gdb) p/x *(unsigned int*)0x0C002000
	//(gdb) p/x PLIC_ENABLE
	uint32_t enable = PLIC_ENABLE;


	//Примеры записи в plic.threshold
	//(gdb) set PLIC_THRESHOLD = 2
	SetPlicThreshold(1);	//0x1
	SetPlicThreshold(0);	//0x0
	SetPlicThreshold(3);	//0x3
	SetPlicThreshold(6);	//0x2 (WARL)
	SetPlicThreshold(4);	//0x0 (WARL)
	SetPlicThreshold(9);	//0x1 (WARL)

	//Примеры чтение plic.threshold
	//(gdb) p/x PLIC_THRESHOLD
	uint32_t threshold = PLIC_THRESHOLD;


	//Примеры чтение plic.pending (read only)
	//(gdb) p/x PLIC_PENDING
	uint32_t pending = PLIC_PENDING;
	uint32_t _pending = GetPlicPending();
*/

	SetHandler(&handler);

//==================================================
//Настройка PLIC

	PlicClearPending();

	SetPlicThreshold(0); //2
	SetPlicPriority(1, 2);
	SetPlicPriority(2, 2);
	SetPlicPriority(3, 3); //3

	SetPlicEnable(3, ENABLE);
	SetPlicEnable(2, ENABLE);
	SetPlicEnable(1, ENABLE);

	EnableExternalInterrupt();

//	Timer timer;
//	timer.StartTimer(10);
//==================================================

//==================================================
//CLINT software interrupt

//	SetClintSoftwareInterrupt();
//	EnableClintSoftwareInterrupt();
//==================================================

//==================================================
//CLINT timer

//	SetClintTimer(1000000);
//	EnableClintTimerInterrupt();
//==================================================


	int i = 1;
//		while(1)
//		{
//			i++;
//			i -= 2;
//		};
	i++;
	asm("wfi");
	i++;

	//asm(".word 0x80ffffff"); //exception: illegal instruction
	//EnableExternalInterrupt();
	i++;
	asm("wfi");
	i++;

	//EnableExternalInterrupt();
	i++;
	asm("wfi");
	i++;

	return 0;
}
