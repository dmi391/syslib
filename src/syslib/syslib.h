/*
 * syslib.h
 *
 * Author: PehotinDO
 */

#ifndef SYSLIB_SYSLIB_H_
#define SYSLIB_SYSLIB_H_

extern "C" inline void set_mtvec(void (*ptr)(void));

extern "C" void handler(void) __attribute__((interrupt)); //__attribute__((section(".interrupt_handler"), interrupt));
extern "C" void enother_handler(void) __attribute__((interrupt));

extern "C" inline void set_mepc(void (*ptr)(void));

extern "C" void init(void);

void get_interrupt_diag(void);
extern "C" void clean_interrupt_diag(void);
typedef struct
{
	unsigned long int mcause;
	unsigned long int mepc;
	unsigned long int mtval;
	unsigned long int mie;
	unsigned long int mip;

}_interrupt_diag;

#endif /* SYSLIB_SYSLIB_H_ */
