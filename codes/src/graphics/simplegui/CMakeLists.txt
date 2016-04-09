file(GLOB_RECURSE sourceFiles ./*.cpp)
file(GLOB_RECURSE headerFiles ./*.h)

if(share_library)
	tng_shared_library(gui ${sourceFiles} ${headerFiles})
else()
	tng_library(gui ${sourceFiles} ${headerFiles})
endif()
target_link_libraries(gui core)

