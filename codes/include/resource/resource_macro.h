#ifndef _RESOURCE_MGR_H_
#define _RESOURCE_MGR_H_

#ifdef _WIN32
#ifdef resource_EXPORTS
#define RES_API		__declspec(dllexport)
#define RES_FUNC_API	extern "C" __declspec( dllexport )
#define RES_GLOBAL_API	extern __declspec( dllexport )
#elif defined(_LIB)
#define RES_API
#define RES_FUNC_API	extern "C"
#define RES_GLOBAL_API 	extern
#else
#define RES_API		__declspec( dllimport )
#define RES_FUNC_API	extern "C" __declspec( dllimport )
#define RES_GLOBAL_API	extern __declspec( dllimport )
#endif// 
#else
#ifdef resource_EXPORTS
#define RES_API			__attribute__ ((visibility("default")))
#define RES_FUNC_API		__attribute__ ((visibility("default")))
#define RES_GLOBAL_API		__attribute__ ((visibility("default")))
#elif defined(_LIB)
#define RES_API
#define RES_FUNC_API	extern "C"
#define RES_GLOBAL_API 	extern
#else
#define RES_API
#define RES_FUNC_API
#define RES_GLOBAL_API
#endif
#endif//  


#endif