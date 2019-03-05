cls
@echo off
echo This patch file for STM32 20-21-45-46 G-EVAL board : MB786 revB 
echo Entering SystemMemory mode and dumping 1024KB of Flash  to c:\upload_old.hex
echo Then reset the board and boot from user Flash
STMFlashLoader.exe -c --pn 1 --br 115200 --db 8 --pr EVEN --sb 1 --ec OFF  --to 10000 --co ON -Dtr --Hi -Rts --Lo  -i STM32F2_1024K -u --fn c:\upload_old.hex  -Dtr --Lo -Rts --Hi --Lo
PAUSE 