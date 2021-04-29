# Библиотека syslib

## syslib содержит:
* Стартфайл sturtup.S
* Функции для работы с прерываниями
* Другие функции

Для использования syslib нужно подключить `"syslib/syslib.h"`
Проект собирать с `-nostartfiles`.

## Описание функций

Функция `_init()` вызывается внутри `__libc_init_array()`, при этом `_init()` - пользовательская функция, содержащая какие-либо дополнительные действия для подготовки или инициализации системы перед работой. Функция `_init()` должна быть объявлена и определена пользователем, без неё проект не будет линковаться. В данной реализации символ `_init` объявлен и определен прямо в startup.S по умолчанию это пустая функция.

Функция `set_mtvec()` назначает обработчик прерываений: записывает адрес обработчика в mtvec. Вызывается в стартфайле и назначает обработчик `handler()`; может вызываться в коде программы для переназначения обработчика.
Внимание: нужно использовать версию компилятора, которая не игнорирует `__attribute__((interrupt))`. (Например версия gcc-2018.07.0-x86_64_...)
Пример:
    set_mtvec((void(*)(void))&enother_handler);

Функция `get_interrupt_diag()` вызывается в обработчике прерывания и формирует диагностику на случай исключения: переписывает регистры mcause, mepc, mtval, mie, mip в глобальную структуру interrupt_diag, расположенную в секции .diag_section. 

Функция `set_mepc()` назначает новое значение epc, если из обработчика прерывания нужно вернуться в другое место (не в то, на котором возникло прерывание).
Пример:
    set_mepc((void(*)(void))&enother_handler);


## Сборка и линковка

Toolchain: RISC-V GCC/Newlib (riscv64-unknown-elf-gcc)

Так как `__libc_init_array()` является частью стандартной библиотеки, то линковать нужно с newlib. Project -> Properties -> C++ Linker -> Libraries -> Library search path (-L):
			`"\...\riscv64-unknown-elf\lib\rv64imafdc\lp64d"`

Ключи для ld: Project -> Properties -> C++ Linker -> General:
			`-nostartfiles`

Можно добавить ключи оптимизации для C++ компилятора:
			`-fno-exceptions`
			`-fno-unwind-tables`
			`-fno-use-cxa-atexit` (деструкторы static объектов)[7]
			`-fno-threadsafe-statics`
			`-Xlinker --gc-sections` (Remove unused sections). Возможно будет эффективнее с `-fdata-sections` (создание отдельных секций под каждую функцию).
			`-fno-rtti` (dinamic cast)
