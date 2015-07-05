set GCC=C:\Tools\arduino-nightly\hardware\tools\avr\bin\avr-gcc
set OBJCOPY=C:\Tools\arduino-nightly\hardware\tools\avr\bin\avr-objcopy
set DUDE=C:\Tools\arduino-nightly\hardware\tools\avr\bin\avrdude
set GCC_FLAGS=-Wall -Werror -g -MMD -Os -mmcu=attiny2313 -DF_CPU=8000000L -std=gnu99

%GCC% %GCC_FLAGS% -E -o out\binary.E src\main.c 
if ERRORLEVEL 1 goto label_exit

%GCC% %GCC_FLAGS% -S -o out\binary.S src\main.c 
if ERRORLEVEL 1 goto label_exit

%GCC% %GCC_FLAGS% -c -o out\binary.o src\main.c
if ERRORLEVEL 1 goto label_exit

%GCC% -w -Wl,--gc-sections -Os -mmcu=attiny2313 -o out\binary.elf out\binary.o
if ERRORLEVEL 1 goto label_exit

%OBJCOPY% -O ihex -R .eeprom out\binary.elf out\binary.hex
if ERRORLEVEL 1 goto label_exit

%DUDE% -CC:\Tools\arduino-nightly\hardware\tools\avr/etc/avrdude.conf -v -pattiny2313 -cstk500v1 -PCOM4 -b19200 -Uflash:w:out\binary.hex:i

:label_exit