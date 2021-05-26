# Библиотека syslib

## syslib содержит:
* Стартфайл startup.S
* Функции для работы с прерываниями (драйверы)
* Другие функции

Для использования syslib нужно подключить `"syslib/syslib.h"`
Проект собирать с `-nostartfiles`.

## Описание

Функция `_init()` вызывается внутри `__libc_init_array()` в startup.S, при этом `_init()` - пользовательская функция, содержащая какие-либо дополнительные действия для подготовки или инициализации системы перед работой. Функция `_init()` должна быть объявлена и определена пользователем, без неё проект не будет линковаться. В данной реализации символ `_init` объявлен и определен прямо в startup.S по умолчанию это пустая функция.

main.cpp содержит примеры вызовов функций.


## Сборка и линковка

Toolchain: RISC-V GCC/Newlib (riscv64-unknown-elf-gcc).

Нужно использовать версию компилятора, которая не игнорирует `__attribute__((interrupt))`. (Например версия gcc-2018.07.0-x86_64_...)

Так как `__libc_init_array()` является частью стандартной библиотеки, то линковать нужно с newlib. Project -> Properties -> C++ Linker -> Libraries -> Library search path (-L):
			`"\...\riscv64-unknown-elf\lib\rv64imafdc\lp64d"`

Ключи для ld: Project -> Properties -> C++ Linker -> General:
			`-nostartfiles`

Можно добавить ключи оптимизации для C++ компилятора:
* `-fno-exceptions`
* `-fno-unwind-tables`
* `-fno-use-cxa-atexit` (деструкторы static объектов)
* `-fno-threadsafe-statics`
* `-Xlinker --gc-sections` (Remove unused sections). Возможно будет эффективнее с `-fdata-sections` (создание отдельных секций под каждую функцию).
* `-fno-rtti` (dinamic cast)

