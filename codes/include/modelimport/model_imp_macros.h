#ifndef _TNG_MODEL_IMP_MAC_H_
#define _TNG_MODEL_IMP_MAC_H_

#ifdef _WIN32
#ifdef plugin_model_imp_EXPORTS
#define MODEL_API		__declspec(dllexport)
#define MODEL_FUNC_API	extern "C" __declspec( dllexport )
#define MODEL_GLOBAL_API	extern __declspec( dllexport )
#elif defined(_LIB)
#define MODEL_API
#define MODEL_FUNC_API	extern "C"
#define MODEL_GLOBAL_API 	extern
#else
#define MODEL_API		__declspec( dllimport )
#define MODEL_FUNC_API	extern "C" __declspec( dllimport )
#define MODEL_GLOBAL_API	extern __declspec( dllimport )
#endif// 
#else
#ifdef plugin_model_imp_EXPORTS
#define MODEL_API			__attribute__ ((visibility("default")))
#define MODEL_FUNC_API		__attribute__ ((visibility("default")))
#define MODEL_GLOBAL_API		__attribute__ ((visibility("default")))
#elif defined(_LIB)
#define MODEL_API
#define MODEL_FUNC_API	extern "C"
#define MODEL_GLOBAL_API 	extern
#else
#define MODEL_API
#define MODEL_FUNC_API
#define MODEL_GLOBAL_API
#endif
#endif//  


#endif