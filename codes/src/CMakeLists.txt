#core project
include_directories(../../dep/uv/include ../../dep/uv/include/uv-private ../../dep/uv/src)
set(UV_COMMON_SRCS
    ../../dep/uv/src/uv-common.c
    )


set(UV_WIN32_SRCS
    ../../dep/uv/src/win/thread.c
    ../../dep/uv/src/win/util.c
    ../../dep/uv/src/win/winapi.c
    )

set(UV_POSIX_SRCS
    ../../dep/uv/src/unix/thread.c
    )
if(WIN32)
    set(UV_SRCS ${UV_WIN32_SRCS} ${UV_COMMON_SRCS})
else()
 if(APPLE)
        set(UV_SRCS
			../../dep/uv/src/unix/darwin.c
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			../../dep/uv/src/unix/linux-core.c
			)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
     elseif(CMAKE_SYSTEM_NAME STREQUAL "DragonFly")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
	elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "solaris")
        set(UV_SRCS
            ${UV_POSIX_SRCS}
            ${UV_COMMON_SRCS}
			)
    endif()
endif()
set(core_src	core/os.cpp
				core/stringfun.cpp
				core/exception.cpp
				core/filesystem.cpp
				core/path.cpp
				core/environment_var.cpp
				core/class_desc.cpp
				core/threadfun.cpp
				core/log.cpp
				core/table_paser.cpp
				core/filestream.cpp
				core/binary_reader_writer.cpp
				core/core_init.cpp
				core/object.cpp
				core/platform_android.cpp
				core/platform_win.cpp
				core/platform_unix.cpp
				core/math.cpp
				core/encode.cpp)
set(core_head	../include/core/os.h
				../include/core/config.h
				../include/core/signal.h
				../include/core/delegate.h
				../include/core/arch_detect.h
				../include/core/autoptr.h
				../include/core/runtime_check.h
				../include/core/core_exports.h
				../include/core/core_macros.h
				../include/core/exception.h
				../include/core/filesystem.h
				../include/core/filestream.h
				../include/core/binary_reader_writer.h
				../include/core/refobject.h
				../include/core/stringfun.h
				../include/core/threadfun.h
				../include/core/atomicfun.h
				../include/core/typeList.h
				../include/core/stringfun.h
				../include/core/types.h
				../include/core/path.h
				../include/core/environment_var.h
				../include/core/property.h
				../include/core/variant.h
				../include/core/object.h
				../include/core/class_desc.h
				../include/core/math.h
				../include/core/log.h
				../include/core/timer.h
				../include/core/buffer_alloc.h
				../include/core/table_paser.h
				../include/core/encode.h)
if(ANDROID)
set(core_src ${core_src} core/android_native_app_glue.c)
endif()
if(share_library)
	tng_shared_library(core ${core_src} ${UV_SRCS} ${core_head})
else()
	tng_library(core ${core_src} ${UV_SRCS} ${core_head})
endif()
target_link_libraries(core liblz4)
if(Jemalloc_FOUND)
target_link_libraries(core jemalloc)
endif()
target_link_libraries(core MathGeoLib)

set (resource_head
	../include/resource/resource.h
	../include/resource/resource_macro.h
	../include/resource/resource_manger.h
	../include/resource/stream_loader.h
	../include/resource/asset_bundle.h
	../include/resource/stream_res.h
	../include/resource/packed_stream_loader.h)
set (resource_src 
	resource/resource.cpp
	resource/stream_loader.cpp
	resource/resource_manger.cpp
	resource/asset_bundle.cpp
	resource/packed_stream_loader.cpp)
if(share_library)
	tng_shared_library(resource ${resource_src} ${resource_head})
else()
	tng_library(resource ${resource_src} ${resource_head})
endif()
target_link_libraries(resource engine)

#engine Project
set(engine_head		../include/engine/engine.h
					../include/engine/engine_macros.h
					../include/engine/service.h
					../include/engine/threadloop.h
					../include/engine/event.h
					
					../include/engine/thread_pool.h
					../include/engine/networkservice.h
					../include/engine/event_dispatcher.h)
set(engine_src		engine/engine.cpp
					engine/threadloop.cpp
					engine/networkservice.cpp
			engine/service.cpp
			engine/event.cpp
			engine/event_dispatcher.cpp
			engine/thread_pool.cpp
			engine/engine_init.cpp)
if(share_library)
	tng_shared_library(engine ${engine_src} ${engine_head})
else()
	tng_library(engine ${engine_src} ${engine_head})
endif()
target_link_libraries(engine core)



#graphics Project
set(graphics_head	../include/graphics/graphics.h
					../include/graphics/window.h
					../include/graphics/graphics_macros.h
					../include/graphics/scene.h
					../include/graphics/scenemanger.h
					../include/graphics/scenenode.h
					../include/graphics/scenenode.inl
					../include/graphics/render.h
					../include/graphics/renderservice.h
					../include/graphics/renderbuffer.h
					../include/graphics/renderindexbuffer.h
					../include/graphics/rendervertexbuffer.h
					../include/graphics/rendermaterial.h
					../include/graphics/renderstate.h
					../include/graphics/rendertexture.h
					../include/graphics/shader.h
					../include/graphics/model.h
					../include/graphics/mesh.h
					../include/graphics/rendercontex.h
					../include/graphics/scenerender.h
					../include/graphics/camera.h
					../include/graphics/color.h
					../include/graphics/texture.h
					../include/graphics/pixelformat.h
					../include/graphics/material_expression.h
					../include/graphics/material_compiler.h
					../include/graphics/input.h
					../include/graphics/pixelformat_desc.h)
					

set(graphics_src	graphics/graphics.cpp
					graphics/input.cpp
					graphics/window.cpp
					graphics/scene.cpp
					graphics/scenemanger.cpp
					graphics/scenenode.cpp
					graphics/render.cpp
					graphics/renderservice.cpp
					graphics/model.cpp
					graphics/mesh.cpp
					graphics/rendercontex.cpp
					graphics/scenerender.cpp
					graphics/camera.cpp
					graphics/color.cpp
					graphics/rendermaterial.cpp
					graphics/rendervertexbuffer.cpp
					graphics/texture.cpp
					graphics/pixelformat.cpp
					graphics/material_expression.cpp
					graphics/graphics_init.cpp
					graphics/shader.cpp
					#graphics/gui_render.cpp
					graphics/platform_android.cpp
					graphics/platform_win.cpp
					graphics/platform_unix.cpp
					graphics/material_compiler.cpp)

if(__TOS_MACOS__)
					set (${graphics_src} ${graphics_src} graphics/window.mm)
endif(__TOS_MACOS__)
if(share_library)
	tng_shared_library(graphics ${graphics_src} ${graphics_head})
else()
	tng_library(graphics ${graphics_src} ${graphics_head})
endif()
#subdirs(graphics/gui)
#subdirs(graphics/simplegui)
target_link_libraries(graphics resource)
#target_link_libraries(graphics gui)
macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)

subdirs(tool)