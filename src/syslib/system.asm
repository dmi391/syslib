# extern "C"
# {
#	 void setup_stack_pointer(long unsigned int);
#	 unsigned long int cycle(void);
#	 void write_result(int number);
#	 void cycle_clear(void);
# }
.align 8

.global setup_stack_pointer
.global cycle_clear
.global cycle
.global WaitingByMcycle
.global write_result
.global prob_timer
.global mstatus_fs


MTIME_K:
.dword 0x3EBA36E2EB1C432D //= 0,0000015625 = 100 / 64000000 hz
MCYCLE_K:
//.dword 0x3E50C6F7A0B5ED8D //= 0,000000015625 = 1/64000000 hz
.dword 0x3E55798EE2308C3A //= 0,00000002 s = 1 / 50 MHz


setup_stack_pointer:
	mv x2, a0
	ret


FF64:
.dword 0xffffffffffffffff
.global cycle_clear
cycle_clear:
	ld t0, FF64
	csrrc x0, mcycle, t0
	ret


cycle:
	csrrs a0, mcycle, x0
	ret


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


mstatus_fs:
	csrrs x11, mstatus, x0
	li x10, 0x2000
	or x11, x10, x11
	CSRRS x0, mstatus, x11
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


.global simple_trap
simple_trap:
	//����� ��������� ���������� -> mtvec
	la 			t0, trap_label
	csrrs 		x0, mtvec, t0
	ret
	//�������
	trap_label:
	csrrs 		t0, mcause, x10 ///����������????
	csrrs 		t1, mie, x0
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
set_mtvec:
	csrw mtvec, a0
	ret


.global set_mepc
set_mepc:
	csrw mepc, a0
	ret
