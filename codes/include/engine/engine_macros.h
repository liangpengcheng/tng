#pragma once
#ifdef _WIN32
#ifdef engine_EXPORTS
#define ENGINE_API		__declspec(dllexport)
#define ENGINE_FUNC_API	extern "C" __declspec( dllexport )
#define ENGINE_GLOBAL_API	extern __declspec( dllexport )
#elif defined(_LIB)
#define ENGINE_API
#define ENGINE_FUNC_API	extern "C"
#define ENGINE_GLOBAL_API 	extern
#else
#define ENGINE_API		__declspec( dllimport )
#define ENGINE_FUNC_API	extern "C" __declspec( dllimport )
#define ENGINE_GLOBAL_API	extern __declspec( dllimport )
#endif// 
#else
#ifdef engine_EXPORTS
#define ENGINE_API			__attribute__ ((visibility("default")))
#define ENGINE_FUNC_API		__attribute__ ((visibility("default")))
#define ENGINE_GLOBAL_API		__attribute__ ((visibility("default")))
#elif defined(_LIB)
#define ENGINE_API
#define ENGINE_FUNC_API	extern "C"
#define ENGINE_GLOBAL_API 	extern
#else
#define ENGINE_API
#define ENGINE_FUNC_API
#define ENGINE_GLOBAL_API
#endif
#endif//  
