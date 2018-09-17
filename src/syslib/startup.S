/*
 * startup.S
 *
 * Author: PehotinDO
 */


	.section .init
	.globl _start
	.type _start,@function

_start:

.option push
.option norelax
	la gp, __global_pointer$
.option pop
	la sp, _sp

/* Обнуление секции .bss */
	la a0, __bss_start
	la a1, __bss_end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:

/* Call global constructors */ //???
/*
	la a0, __libc_fini_array
	call atexit
	call __libc_init_array
*/

/* Включение FPU (из system.asm) */
	call mstatus_fs
//	/* Enable FPU */
//	li t0, MSTATUS_FS //0x2000
//	csrs mstatus, t0
//	csrr t1, mstatus
//	and t1, t1, t0
//	beqz t1, 1f
//	fssr x0
//1:

/* Настройка mtvec: Обработчик прерываний */
	la a0, handler
	call set_mtvec

/* Обнуление диагностики прерываний*/
	call clean_interrupt_diag

/* Инициализация подсистем */
	call init

/* argc = argv = 0 */
	li a0, 0
	li a1, 0
	call main
	tail exit
1:
	j 1b
