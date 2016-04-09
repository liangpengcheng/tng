
add_definitions(-DSTRICT
-DSTRICT
-DWIN32_LEAN_AND_MEAN
-DGTEST_HAS_PTHREAD=0
-D_HAS_EXCEPTIONS=1
)
if( MSVC ) # VS2012 doesn't support correctly the tuples yet
     	    add_definitions( /D _VARIADIC_MAX=10 )
endif()
tng_exe(unit_test unit_test.cpp
						core_test.h
						engine_test.h)
target_link_libraries(unit_test gtest)
target_link_libraries(unit_test core)
target_link_libraries(unit_test engine)