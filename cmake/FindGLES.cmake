find_path(GLES2_INC_DIR GLES2/gl2.h
	  HINTS $ENV(GLES2DIR)
	  PATH_SUFFIXES include
	  PATHS /usr/local
	  /usr
	  /sw
	  /opt/local
	  /opt/csw
	  /opt
	  ~/Library/Frameworks
	  /Library/Frameworks
	)
	find_library(GLES2_LIBRARY
	GLESv2
	HINTS $ENV(GLES2DIR)
	PATH_SUFFIXES lib64 lib
	PATHS ~/Library/Frameworks
	  /Library/Frameworks
	  /usr/local
	  /usr
	  /usr/X11R6
	  /opt)
if(ANDROID)
	if(GLES2_INC_DIR)
	  MESSAGE(STATUS "Found GLES2 in ${GLES2_INC_DIR}")
	  if(NOT EMCC)
		include_directories ( ${GLES2_INC_DIR} )
		set(link_libs ${link_libs} ${GLES2_LIBRARY})
	  endif()
	endif()
endif()
	##
	find_library(GLE_LIBRARY
	GL
	HINTS $ENV(GL)
	PATH_SUFFIXES lib64 lib
	PATHS ~/Library/Frameworks
	  /Library/Frameworks
	  /usr/local
	  /usr
	  /usr/X11R6
	  /opt)

	if(GLE_LIBRARY)
	  MESSAGE(STATUS "Found GL  ${GLE_LIBRARY}")
	  set(link_libs ${link_libs} ${GLE_LIBRARY})
	  add_definitions(-DHASXGL=1)
	endif()