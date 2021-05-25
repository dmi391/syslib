/*
 * syslib.h
 *
 * 		Author: PehotinDO
 */

#ifndef SYSLIB_SYSLIB_H_
#define SYSLIB_SYSLIB_H_

#include <cstdint>
#include "interrupts/interrupt.h"


//==================================================
//CSRs

#define ReadCsr(reg) ({ \
    register unsigned long readval; \
    asm volatile ("csrr %[in], " #reg :[in] "=r"(readval) ::); \
    readval; })


#define WriteCsr(reg, val) ({ \
    asm volatile ("csrw "#reg", %[out]" ::[out] "r"(val):); })


#define SetBitsCsr(reg, bits) ({ \
	asm volatile ("csrs "#reg", %[out]" ::[out] "r"(bits):); })


#define ClearBitsCsr(reg, bits) ({ \
	asm volatile ("csrc "#reg", %[out]" ::[out] "r"(bits):); })

//==================================================


#endif /* SYSLIB_SYSLIB_H_ */
