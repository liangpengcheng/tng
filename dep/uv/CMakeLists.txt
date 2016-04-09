include_directories(include include/uv-private src)
add_definitions(-DHAVE_CONFIG_H)

set(UV_COMMON_SRCS
    src/uv-common.c
    )


set(UV_WIN32_SRCS
    src/win/thread.c
    src/win/util.c
    src/win/winapi.c
    )

set(UV_POSIX_SRCS
    src/unix/thread.c
    )

# set UV_SRCS and UV_LIBS

if(WIN32)
    include_directories(src/ares/config_win32)
    add_definitions(-D_WIN32_WINNT=0x0502 -DEIO_STACKSIZE=262144 -D_GNU_SOURCE)
    set(UV_SRCS ${UV_WIN32_SRCS} ${UV_COMMON_SRCS})
	#set(UV_LIBS ws2_32 psapi.lib iphlpapi.lib)
else()
    # POSIX common
    include_directories(src/unix/ev)
    add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -DEIO_STACKSIZE=262144)
	
    # OS specific thingy
    # FIXME: solaris here
    if(APPLE)
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            src/unix/darwin.c
            #src/unix/fsevents.c
			#src/unix/proctitle.c
			#src/unix/darwin-proctitle.c
            #src/unix/kqueue.c
			)
       # set(UV_LIBS /System/Library/Frameworks/CoreServices.framework)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			src/unix/linux-core.c
			#src/unix/linux-syscalls.c
			#src/unix/linux-inotify.c
			#src/unix/proctitle.c
			)
       # set(UV_LIBS rt pthread m dl)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            #src/unix/aix.c
			)
       # set(UV_LIBS perfstat)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            #src/unix/freebsd.c
            #src/unix/kqueue.c
			)
		#if(FLASCC)
		#	set(UV_LIBS)
		#else()
		#	set(UV_LIBS m kvm pthread)
		#endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            #src/unix/openbsd.c
            #src/unix/kqueue.c
			)
			#set(UV_LIBS m kvm pthread)
     elseif(CMAKE_SYSTEM_NAME STREQUAL "DragonFly")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            #src/unix/freebsd.c
            #src/unix/kqueue.c
			)
        #set(UV_LIBS m kvm pthread)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            #src/unix/netbsd.c
            #src/unix/kqueue.c
			)
        #set(UV_LIBS m kvm pthread nptl)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "solaris")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
            #src/unix/sunos.c
			)
       # set(UV_LIBS nsl socket kstat)
    endif()
endif()

#if( share_library )
#tng_shared_library(uv ${UV_SRCS})
#else()
#tng_library(uv ${UV_SRCS})
#endif()