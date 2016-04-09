#pragma once
#ifdef _WIN32
#ifdef core_EXPORTS
#define CORE_API		__declspec(dllexport)
#define CORE_FUNC_API	extern "C" __declspec( dllexport )
#define CORE_GLOBAL_API	extern __declspec( dllexport )
#elif defined(_LIB)
#define CORE_API
#define CORE_FUNC_API	extern "C"
#define CORE_GLOBAL_API 	extern
#else
#define CORE_API		__declspec( dllimport )
#define CORE_FUNC_API	extern "C" __declspec( dllimport )
#define CORE_GLOBAL_API	extern __declspec( dllimport )
#endif// 
#else
#ifdef core_EXPORTS
#define CORE_API			__attribute__ ((visibility("default")))
#define CORE_FUNC_API		__attribute__ ((visibility("default")))
#define CORE_GLOBAL_API		__attribute__ ((visibility("default")))
#elif defined(_LIB)
#define CORE_API
#define CORE_FUNC_API	extern "C"
#define CORE_GLOBAL_API 	extern
#else
#define CORE_API
#define CORE_FUNC_API
#define CORE_GLOBAL_API
#endif
#endif//  
