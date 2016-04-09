cmake -DCMAKE_TOOLCHAIN_FILE=%EMSCRIPTEN%\cmake\Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Debug -Dbuild_gles=on -Dbuild_sample=on -DCMAKE_MAKE_PROGRAM=D:\Emscripten\mingw\4.6.2_32bit\mingw32-make.exe -DEMCC=1 -G "MinGW Makefiles" ../..
pause

