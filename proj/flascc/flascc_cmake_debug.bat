@echo off
del cmakecache.txt

set PTH=%PATH%
:: Make sure no custom cygwins are present in PATH. 
set PATH=%PATH:cygwin=cygwin_unused%
:: git cannot be present in PATH when mingw32-make is invoked! Or otherwise it fails with some DLL confusion!
set PATH=%PATH:git=git_unused%

echo Assuming FlasCC is at E:/Crossbridge_1.0.1
set PATH=e:/Crossbridge_1.0.1/cygwin/bin;%PATH%
e:/Crossbridge_1.0.1/sdk/usr/bin/gcc.exe -v
cmake -DFLASCC=1 -DCMAKE_SYSTEM_NAME=FreeBSD -DCMAKE_CXX_COMPILER_WORKS=TRUE -DCMAKE_C_COMPILER_WORKS=TRUE -DCMAKE_C_COMPILER=e:/Crossbridge_1.0.1/sdk/usr/bin/gcc.exe -DCMAKE_CXX_COMPILER=e:/Crossbridge_1.0.1/sdk/usr/bin/g++.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=e:/Crossbridge_1.0.1/sdk/usr/bin/make.exe -G "Unix Makefiles" ../..
set PATH=%PTH%
set PTH=

pause

