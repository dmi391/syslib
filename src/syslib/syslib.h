/*
 * syslib.h
 *
 * Author: PehotinDO
 */

#ifndef SYSLIB_SYSLIB_H_
#define SYSLIB_SYSLIB_H_

extern "C" inline void set_mtvec(void (*ptr)(void));

extern "C" void handler(void) __attribute__((interrupt));
extern "C" void enother_handler(void) __attribute__((interrupt));

extern "C" inline void set_mepc(void (*ptr)(void));

extern "C" void init(void);

extern inline void __attribute__((always_inline))  get_interrupt_diag(void);

extern inline void __attribute__((always_inline)) void_interrupt_compleat(void);

void interrupt_compleat(void);


void EnableExternInterrupt(void);
void DisableExternInterrupt(void);

/**
 * Диагностика при возникновении прерывания/исключения (глобально в секции .diag_section)
 */
typedef struct
{
	unsigned long int mcause;
	unsigned long int mepc;
	unsigned long int mtval;
	unsigned long int mie;
	unsigned long int mip;

}_interrupt_diag;


#endif /* SYSLIB_SYSLIB_H_ */
