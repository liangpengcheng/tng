file(GLOB_RECURSE sourceFiles ./*.c)
file(GLOB_RECURSE headerFiles ./*.h)
tng_library(spine-c ${sourceFiles} ${headerFiles})

