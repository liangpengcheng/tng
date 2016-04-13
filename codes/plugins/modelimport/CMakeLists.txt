if(NOT ANDROID)
	set(model_imp_src
		../../include/modelimport/model_imp.h 
		../../include/modelimport/texture_imp.h 
		../../include/modelimport/model_imp_macros.h 
		../../include/modelimport/atlas_imp.h
		../../include/modelimport/port.h
		model_imp.cpp
		texture_imp.cpp
		atlas_imp.cpp
		port.cpp)
	if(share_library)
			tng_shared_library(plugin_model_imp ${model_imp_src})
	else()
			tng_library(plugin_model_imp ${model_imp_src})
	endif()
	target_link_libraries(plugin_model_imp resource)
	target_link_libraries(plugin_model_imp assimp)
	target_link_libraries(plugin_model_imp resil)
	target_link_libraries(plugin_model_imp graphics)
endif()