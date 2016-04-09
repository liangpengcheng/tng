#pragma once
#include "core_exports.h"
#include "arch_detect.h"

#define BIT(B) 1 << (B)
#define NANOTOSECOND(t) (f64(t)/1000000000.0f)
#define SECONDTONANO(t) (f64(t)*1000000000.0f)
#ifdef _MSC_VER
#pragma warning (disable:4251)
#pragma warning (disable:4250)
#endif

#ifdef SAFE_DELETE
#undef SAFE_DELETE
#endif // SAFE_DELETE

#ifdef SAFE_DELETE_ARRAY
#undef SAFE_DELETE_ARRAY
#endif // SAFE_DELETE_ARRAY

#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif // SAFE_RELEASE

#define SAFE_DELETE(x) if(x)\
	delete x;
#define SAFE_DELETE_ARRAY(x) if(x)\
	delete []x;
#define SAFE_RELEASE(x) if(x)\
	x->Release();

#ifndef FORCEINLINE
#if defined(_MSC_VER)
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __inline __attribute__ ((always_inline))
#endif
#endif
#ifndef NOINLINE
#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__ ((noinline))
#endif
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif // !MAX_PATH

#ifdef TNG_OS_FAMILY_WINDOWS
#define MAIN \
	int main(int argc, const char **argv)
#elif defined(TNG_ANDROID)
#define MAIN \
	void android_main(struct android_app* state)
#elif defined(TNG_IPHONE)
#define MAIN \
	void main(int argc,const char **argv)
#else
#define MAIN \
	int main(int argc,const char **argv)
#endif // TNG_OS_FAMILY_WINDOWS

#define TNG_OVERRIDE override