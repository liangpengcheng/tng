
tng_game(sample_gfx gfx_sample.cpp)
target_link_libraries(sample_gfx plugin_gles)

if(NOT ANDROID)
	tng_game(sample_model_imp model_imp_sample.cpp)
	target_link_libraries(sample_model_imp plugin_gles)
	target_link_libraries(sample_model_imp plugin_model_imp)
endif()

set(CURRENT_SHARED_LIBRARIES
	"${LIBRARY_OUTPUT_PATH}/libsample_gfx.so"
)
if(ANDROID)
android_create_apk("sample_gfx" "${CMAKE_BINARY_DIR}/apk" "${CURRENT_SHARED_LIBRARIES}" 
"${EngineAssetDir}"  "${SampleAssetDir}" "${JavaSrcDir}")
endif()