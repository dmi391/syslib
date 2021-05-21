/*
 * clint.h
 *
 *      Author: PehotinDO
 */

#ifndef SYSLIB_CLINT_H_
#define SYSLIB_CLINT_H_

#include <cstdint>


//CSRs for interrupts from CLINT
#define CSR_MIE_MTIE			0x0080	//csr mie.mtie
#define CSR_MIE_MSIE			0x0008	//csr mie.msip
#define CSR_MSTATUS_MIE			0x0008	//csr mstatus.mie

extern "C" void EnableClintTimerInterrupt();
extern "C" void DisableClintTimerInterrupt();
extern "C" void EnableClintSoftwareInterrupt();
extern "C" void DisableClintSoftwareInterrupt();


//CLINT fields
#define CLINT_MSIP				*(volatile uint32_t*)0x02000000
#define CLINT_MTIMECMP			*(volatile uint64_t*)0x02004000
#define CLINT_MTIME				*(volatile uint64_t*)0x0200BFF8

extern "C" void SetClintTimer(uint64_t period);
extern "C" void SetClintSoftwareInterrupt();
extern "C" void ClearClintSoftwareInterrupt();


#endif /* SYSLIB_CLINT_H_ */
