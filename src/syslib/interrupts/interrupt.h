/*
 * interrupt.h
 *
 *      Author: PehotinDO
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "clint.h"
#include "plic.h"

#include <setjmp.h>
extern jmp_buf ExceptionBuf;


//Interrupt common
extern "C" void SetHandler(void (*ptrHandler)(void));
extern "C" void SetMepc(void (*ptrReturn)(void));
extern "C" void handler(void) __attribute__((interrupt, aligned(4)));


#endif /* INTERRUPT_H_ */
