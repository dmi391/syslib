/*
 * plic.h
 *
 *      Author: PehotinDO
 */

#ifndef PLIC_H_
#define PLIC_H_

#include <cstdint>


//CSRs for interrupts from PLIC
#define CSR_MIE_MEIE			0x0800	//csr mie.meie
#define CSR_MSTATUS_MIE			0x0008	//csr mstatus.mie

extern "C" void EnableExternalInterrupt();
extern "C" void DisableExternalInterrupt();


//PLIC fields
#define PLIC_PRIORITY			*(volatile uint32_t*)0x0C000000 //1: 0x0C000004; 2: 0x0C000008; 3: 0x0C00000C
	#define PLIC_PRIORITY_1		*(volatile uint32_t*)0x0C000004
	#define PLIC_PRIORITY_2		*(volatile uint32_t*)0x0C000008
	#define PLIC_PRIORITY_3		*(volatile uint32_t*)0x0C00000C
#define PLIC_ENABLE				*(volatile uint32_t*)0x0C002000
#define PLIC_THRESHOLD			*(volatile uint32_t*)0x0C200000
#define PLIC_PENDING			*(volatile uint32_t*)0x0C001000
#define PLIC_CLAIM				*(volatile uint32_t*)0x0C200004

enum state
{
	ENABLE = true,
	DISABLE = false
};

extern "C" void SetPlicPriority(uint8_t id, uint8_t priority);
extern "C" void SetPlicEnable(uint8_t id, state st);
extern "C" void SetPlicThreshold(uint8_t threshold);
extern "C" uint32_t GetPlicPending(void);
extern "C" uint32_t PlicComplete(void);
extern "C" void PlicClearPending(void);


#endif /* PLIC_H_ */
