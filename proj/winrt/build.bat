mkdir build_winrt_arm
cd build_winrt_arm

set msvc_path=C:\Program Files (x86)\Microsoft Visual Studio 12.0

call "%msvc_path%\Common7\Tools\VsDevCmd.bat"
call "%msvc_path%\VC\bin\x86_arm\vcvarsx86_arm.bat"

cmake.exe -G Ninja -DCMAKE_MAKE_PROGRAM=nmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../winrt_toolchain.cmake  %* ..\..\..
pause