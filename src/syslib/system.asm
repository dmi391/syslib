/*
 * system.asm
 *
 * Author: PehotinDO
 */

.align 8

//========================================

MTIME_K:
.dword 0x3EBA36E2EB1C432D //= 0,0000015625 = 100 / 64000000 hz
MCYCLE_K:
//.dword 0x3E50C6F7A0B5ED8D //= 0,000000015625 = 1/64000000 hz
.dword 0x3E55798EE2308C3A //= 0,00000002 s = 1 / 50 MHz

//========================================

.global cycle
cycle:
	csrrs a0, mcycle, x0
	ret


.global WaitingByMcycle
WaitingByMcycle://в fа0 необходимо передать время, которое надо выждать
	csrrs 		t1,  mcycle, zero
	fcvt.d.l 	ft0, t1				//переводим значение mcycle  в double
	li 		    t2, 0
	fld 		ft1, MCYCLE_K, t0   //переписываем в регистр ft1 значение MCYCLE_K
	//fmul.d		ft2, ft1, fa0		//переводим в такты переданное число из Си-функции
	fdiv.d		ft2, fa0, ft1		//переводим в такты переданное число из Си-функции
	fadd.d 		ft2, ft2, ft0		//Значение mcycle с учетом задержки
	loopWaitingByMcycle:
	csrrs 		t1,  mcycle, zero
	fcvt.d.l 	ft3, t1
	flt.d 		t0,  ft2, ft3		//сравниваем значения
	beq 		t0,  zero, loopWaitingByMcycle
	ret


.global get_time_mtime
get_time_mtime:
	//������ �� mtime
	li		t0, 0x200bff8
	ld 		t1, 0(t0)
	//�������������� � double
	fcvt.d.l 	ft0, t1 //ft0 = �������� mtime � double
	//�������� ������������
	//li 		t2, 0
	la			t2, MTIME_K
	fld			ft1, 0(t2)
	//fld 		ft1, MTIME_K, t2
	fmul.d		fa0, ft1, ft0 //fa0 = f10 = �����
	ret


.global get_time_mcycle
get_time_mcycle:
	//������ �� CSR mcycle
	csrrs 		t1, mcycle, x0
	//�������������� � double
	fcvt.d.l 	ft0, t1 //ft0 = �������� mcycle � double
	//�������� ������������
	li 		t2, 0
	fld 		ft1, MCYCLE_K, t2
	fmul.d		fa0, ft1, ft0 //fa0 = f10 = �����
	ret


.global get_minstret
get_minstret:
	//���������� ���������� ����������
	csrrs a0, minstret, x0
	ret


.global debug_stop_counters
debug_stop_counters:
	li 			t0, 0x7B0 //�����
	li			t1, 0x600 //���������
	sd 			t1, 0(t0)
	ret


.global set_mtvec
.type set_mtvec,@function
set_mtvec:
	csrw mtvec, a0
	ret


.global set_mepc
.type set_mepc,@function
set_mepc:
	csrw mepc, a0
	ret
