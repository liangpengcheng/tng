# Define here the needed parameters
set (ASSIMP_SV_REVISION 1264)
set (ASSIMP_VERSION_MAJOR 3)
set (ASSIMP_VERSION_MINOR 0)
set (ASSIMP_VERSION_PATCH ${ASSIMP_SV_REVISION}) # subversion revision?
set (ASSIMP_VERSION ${ASSIMP_VERSION_MAJOR}.${ASSIMP_VERSION_MINOR}.${ASSIMP_VERSION_PATCH})
set (ASSIMP_SOVERSION 3)

SET(ASSIMP_DEBUG_POSTFIX "D" CACHE STRING "Debug Postfitx for lib, samples and tools")

# Allow the user to build a static library
SET ( ASSIMP_BUILD_STATIC_LIB ON)

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CFG_INTDIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
										   
	# Do not consider .c files
	foreach(fname ${Sources}) 
		GET_FILENAME_COMPONENT(fext ${fname} EXT)
		if(fext STREQUAL ".cpp")
			SET_SOURCE_FILES_PROPERTIES(${fname}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")     
		endif(fext STREQUAL ".cpp")
	endforeach(fname) 
										   
  ENDIF(MSVC)
  # Add precompiled header to SourcesVar
  LIST(APPEND ${SourcesVar} ${PrecompiledSource})
	
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

INCLUDE_DIRECTORIES( code/BoostWorkaround )
ADD_DEFINITIONS( -DASSIMP_BUILD_BOOST_WORKAROUND )



SET ( ASSIMP_NO_EXPORT ON)

# Search for external dependencies, and build them from source if not found

  include(CheckIncludeFile)
  include(CheckTypeSize)
  include(CheckFunctionExists)


IF ( ASSIMP_NO_EXPORT )
	ADD_DEFINITIONS( -DASSIMP_BUILD_NO_EXPORT)
	MESSAGE( STATUS "Build an import-only version of Assimp." )
ENDIF( ASSIMP_NO_EXPORT )


IF    ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
ELSE  ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")
	ADD_DEFINITIONS ( -D'ASSIMP_BUILD_ARCHITECTURE="${ASSIMP_BUILD_ARCHITECTURE}"' )
ENDIF ( ASSIMP_BUILD_ARCHITECTURE STREQUAL "")

# ${CMAKE_GENERATOR}
SET ( ASSIMP_BUILD_COMPILER "" CACHE STRING 
	"describe the current compiler."
)
IF    ( ASSIMP_BUILD_COMPILER STREQUAL "")
ELSE  ( ASSIMP_BUILD_COMPILER STREQUAL "")
	ADD_DEFINITIONS ( -D'ASSIMP_BUILD_COMPILER="${ASSIMP_BUILD_COMPILER}"' )
ENDIF ( ASSIMP_BUILD_COMPILER STREQUAL "")

MARK_AS_ADVANCED ( ASSIMP_BUILD_ARCHITECTURE ASSIMP_BUILD_COMPILER )


ADD_SUBDIRECTORY( code/ )
SET ( ASSIMP_BUILD_ASSIMP_TOOLS ON CACHE BOOL
	"If the supplementary tools for Assimp are built in addition to the library."
)
