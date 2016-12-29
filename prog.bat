echo off

set COMPILER = H:\sviluppo_microp\AVR\WinAVR20100110\

set PATH=%PATH%;%COMPILER%avr\bin
set PATH=%PATH%;%COMPILER%bin
SET PATH=%PATH%;%CD%

SET project=XboxRfModule
SET chip=attiny13 


CLS 



echo compiling..................%project% 

echo on

avr-gcc -g -O1 -mmcu=%chip% -c %project%.c

avr-gcc -g -mmcu=%chip% -o %project%.elf %project%.o

avr-objcopy -j .text -j .data -O ihex %project%.elf %project%.hex

echo programming..................%chip% 

avrdude  -c usbtiny -p %chip% -U flash:w:%project%.hex

echo off
del *.hex
del *.o
del *.elf
