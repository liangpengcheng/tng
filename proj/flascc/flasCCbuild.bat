@echo off
set PTH=%PATH%
set PATH=%PATH%;e:/Crossbridge_1.0.1/cygwin\bin

::java -jar E:\Crossbridge_1.0.1\sdk\usr\lib/asc2.jar -merge -md  -AS3 -strict -import E:\Crossbridge_1.0.1\sdk/usr/lib/builtin.abc  -import E:\Crossbridge_1.0.1\sdk/usr/lib/ISpecialFile.abc -import E:\Crossbridge_1.0.1\sdk/usr/lib/CModule.abc -import E:\Crossbridge_1.0.1\sdk/usr/lib/playerglobal.abc Console.as
make
set PATH=%PTH%
set PTH=
pause