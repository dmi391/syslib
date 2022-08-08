# syslib

----

'syslib' contains minimum required for embedded RISC-V project.

## syslib content

* Startfile startup.S
* Ld-script linker.ld
* Functions for work with interrupts (PLIC and CLINT drivers, handlers etc.)
* Others

To use syslib it is necessary to include `syslib/syslib.h`.

## Startfile

Function `_init()` called inside `__libc_init_array()` in startup.S. `_init()` is user defined function which contains any additional actions before `main()`. Function `_init()` must be declared and defined by user otherwise project can't be linked. In this implementation symbol `_init` is declared and defined immediately in startup.S (empty function).

main.cpp contains examples of function calls.

## Build and link

Toolchain: RISC-V GCC/Newlib (riscv64-unknown-elf-gcc).  
Use compiler version which does not ignore `__attribute__((interrupt))` (version gcc-2018.07.0-x86_64_... or newer).

Link with flag `-nostartfiles`.  

`__libc_init_array()` is the part of libc, so it is necessary to link project with newlib:

        `-L"\...\riscv64-unknown-elf\lib\rv64imafdc\lp64d"`

For optimization, it can be used keys:

* `-fno-exceptions`
* `-fno-unwind-tables`
* `-fno-use-cxa-atexit` (static object destructors)
* `-fno-threadsafe-statics`
* `-Xlinker --gc-sections` (Remove unused sections). It will be more effective with `-ffunction-sections` and `-fdata-sections`
* `-fno-rtti` (dynamic cast)

## Quick start

Set actual paths in file '/syslib/paths'.

Build project:

        make

Launch with hardware:

        cd ./launch-sh/
        ./openocd_gdb_launch.sh ../Debug/syslib.elf

Launch with Spike:

        cd ./launch-sh/
        ./spike_openocd_gdb_launch.sh ../Debug/syslib.elf
