/*
 * system.asm
 *
 * Author: PehotinDO
 */

.align 8

//Примеры asm-кода

//==================================================

MTIME_CONST:
.dword 0x3EBA36E2EB1C432D //= 0,0000015625 = 100 / 64000000 Hz

MCYCLE_CONST:
.dword 0x3E50C6F7A0B5ED8D //= 0,000000015625 = 1/64000000 Hz
//.dword 0x3E55798EE2308C3A //= 0,00000002 s = 1 / 50 MHz


.global get_time_mtime
get_time_mtime:
	//Чтение из mtime
	li			t0, 0x200bff8
	ld 			t1, 0(t0)
	//Преобразование в double
	fcvt.d.l 	ft0, t1 //ft0 = Значение mtime в double
	//Умножение на константу
	la			t2, MTIME_CONST
	fld			ft1, 0(t2)
	fmul.d		fa0, ft1, ft0 //fa0 = Время
	ret


.global get_time_mcycle
get_time_mcycle:
	//Чтение из CSR mcycle
	csrrs 		t1, mcycle, x0
	//Преобразование в double
	fcvt.d.l 	ft0, t1 //ft0 = Значение mcycle в double
	//Умножение на константу
	li 			t2, 0
	fld 		ft1, MCYCLE_CONST, t2
	fmul.d		fa0, ft1, ft0 //fa0 = Время
	ret

//==================================================

//Объявление: extern "C" unsigned long get_mcycle(void);
.global get_mcycle
cycle:
	csrrs a0, mcycle, zero
	ret

//==================================================
