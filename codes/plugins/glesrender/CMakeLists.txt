
if(build_gles)
	set(gles_src	../../include/glesrender/gles_render.h
								../../include/glesrender/gles_render_macros.h
								../../include/glesrender/gles_vertexbuffer.h
								../../include/glesrender/gles_indexbuffer.h
								../../include/glesrender/gles_shader.h
								../../include/glesrender/gles_texture.h
							gles_render.cpp
							gles_shader.cpp
							gles_vertexbuffer.cpp
							gles_texture.cpp
							gles_android.cpp
							gles_win32.cpp
							#FxLibGL/CstBufferGL.cpp
							#FxLibGL/FxLibGL.cpp
							#FxLibGL/GLSLProgram.cpp
							#FxLibGL/GLSLShader.cpp
							#FxLibGL/PathRendering.cpp
							#FxLibGL/resourcesGL.cpp
							#FxLibGL/SamplerStateGL.cpp
							#FxLibGL/StateGroupGL.cpp
							#FxLibGL/UniformGL.cpp
							)
	if(FLASCC)
		set(gles_src ${gles_src} gles_flascc.cpp)
	endif()
	if(X11_FOUND)
		set(gles_src ${gles_src} gles_x11.cpp)
	endif()
	if(share_library)
		tng_shared_library(plugin_gles ${gles_src})
	else()
		tng_library(plugin_gles ${gles_src})
	endif()
	target_link_libraries(plugin_gles graphics)
	if(NOT ANDROID)
		target_link_libraries(plugin_gles GLEW)
	endif()
	
endif()