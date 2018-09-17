OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{

  RAM_MEM 				: ORIGIN = 0x10000000, LENGTH = 0xFFFFF
  MEM_DIAG				: ORIGIN = 0x10020010, LENGTH = 0x50
  /*MEM_SOURCE 			: ORIGIN = 0x10020100, LENGTH = 0x3FFFF  
  MEM_RECEIVER 			: ORIGIN = 0x10060100, LENGTH = 0x3FFFF*/
  /*MAS_REG 				: ORIGIN = 0x10130000, LENGTH = 0x1000*/
  TEST_RESULTS			: ORIGIN = 0x100E0100, LENGTH = 0x6FFFF  
  /*MPU_REG 				: ORIGIN = 0x61000000, LENGTH = 0xFF*/
  /*VPU_REG 				: ORIGIN = 0x64000000, LENGTH = 0xFF*/   
  USV_REG 				: ORIGIN = 0x66000000, LENGTH = 0x13F 
  TIMER_REG 			: ORIGIN = 0x66100000, LENGTH = 0x15F
  MAS_REG 				: ORIGIN = 0x67000000, LENGTH = 0x1000
  MEMCT_REG 			: ORIGIN = 0x69000000, LENGTH = 0xFF 
  
}

SECTIONS
{

. = 0x10000000;


.text ALIGN(8) :
{
  *(.init)
  *(.text)
} > RAM_MEM

.interrupt_handler ALIGN(8) :
{
  *(.interrupt_handler)
} > RAM_MEM

.rodata ALIGN(8) :
{
  *(.rodata)
} > RAM_MEM


.data ALIGN(8) :
  {
    *(.data)
  } > RAM_MEM
  .sdata ALIGN(8) :
  {
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata)
  } > RAM_MEM
  
  PROVIDE(_edata = .);


PROVIDE( __bss_start = . );
.bss ALIGN(8) :
{
  *(.sbss)
  *(.bss)
} > RAM_MEM
PROVIDE( __bss_end = . );


  PROVIDE(exit = .);
  _exit = exit;


  PROVIDE( _end = . );
  PROVIDE( end = . );

/* ****** */

/*
. = 0x10001000; 
.testDataVPU_section ALIGN(8) :
{
    *(.testDataVPU_section)
} > VPU_TEST_DATA
*/

  PROVIDE( _sp = 0x10020000);  /* stack */
  
/*Диагностика для формирования при возникновении прерывания*/  
  . = 0x10020010;
  .diag_section ALIGN(8) :
  {
      *(.diag_section)
  } > MEM_DIAG  


/*
. = 0x10020100;
.memorySource_section ALIGN(8) :
{
    *(.memorySource_section)
} > MEM_SOURCE

. = 0x10060100;
.memoryReceiver_section ALIGN(8) :
{
    *(.memoryReceiver_section)
} > MEM_RECEIVER
*/

. = 0x100E0100;
.testResult_section ALIGN(8) :
{
    *(.testResult_section)
} > TEST_RESULTS

/*
. = 0x61000000;
.mpu_section ALIGN(8) :
{
    *(.mpu_section)
} > MPU_REG
*/

/*
. = 0x64000000;
.vpu_section ALIGN(8) :
{
    *(.vpu_section)
} > VPU_REG
*/

. = 0x66000000;
.usv_section ALIGN(8) :
{
    *(.usv_section)
} > USV_REG

. = 0x66100000;
.timer_section ALIGN(8) :
{
    *(.timer_section)
} > TIMER_REG

. = 0x67000000;
.mas_section ALIGN(8) :
{
    *(.mas_section)
} > MAS_REG

/*
. = 0x69000000;
.memct_section ALIGN(8) :
{
    *(.memct_section)
} > MEMCT_REG
*/

 PROVIDE( _sp = 0x10020000);  /*stack*/

}