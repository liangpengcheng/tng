if(dependencies)
subdirs(uv)
if(build_unit_test)
subdirs(gtest-1.6.0)
endif(build_unit_test)
subdirs(MathGeoLib)
subdirs(zlib)
set(ZLIB_FOUND 1)
set(ZLIB_LIBRARIES zlibstatic)
set(ZLIB_INCLUDE_DIR zlib)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

SET( unzip_SRCS
		unzip/crypt.h
		unzip/ioapi.c
		unzip/ioapi.h
		unzip/unzip.c
		unzip/unzip.h
	)

	ADD_LIBRARY( unzip STATIC
			${unzip_SRCS}
		)
if(build_model_importer)
subdirs(assimp)
subdirs(resil)
endif()
if(WIN32)
if(build_gles)
subdirs(glew)
endif(build_gles)
endif(WIN32)

endif(dependencies)
subdirs(lz4)
subdirs(spine)
subdirs(nvg)