set PTH=%PATH%
:: Make sure no custom cygwins are present in PATH. 
set PATH=%PATH:cygwin=cygwin_unused%
:: git cannot be present in PATH when mingw32-make is invoked! Or otherwise it fails with some DLL confusion!
set PATH=%PATH:git=git_unused%

echo Assuming FlasCC is at E:/Crossbridge_1.0.1
set FLASCC_GDB_RUNTIME="C:\Program Files (x86)\Mozilla Firefox\firefox.exe"
set PATH=%PATH%;e:/Crossbridge_1.0.1/cygwin\bin
e:/Crossbridge_1.0.1/sdk/usr/bin/gdb ../../FreeBSD_avm2_unknowncomp_Debug_bin/unit_test.swf
set PATH=%PTH%
set PTH=
set FLASCC_GDB_RUNTIME=
pause
