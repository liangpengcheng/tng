#pragma once
#include <string>
#ifdef MATH_SSE2
#define RAPIDJSON_SSE2
#endif
#include "MathGeoLib/Math/MathTypes.h"
#include "MathGeoLib/SystemInfo.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "returncode.h"
#include "arch_detect.h"
#if TNG_OS_FAMILY_WINDOWS
#else
#include <pthread.h>
#endif
namespace tng
{
#ifdef _WIN32
	//typedef unsigned __int64	u64;
	//typedef __int64				s64;
	typedef		wchar_t			wchar16;
	typedef		int				wchar32;
#else
	typedef		short			wchar16;
	typedef		wchar_t			wchar32;
	//typedef unsigned long long	u64;
	//typedef long long			s64;
#endif
	//typedef		s64				size;
	//typedef     unsigned int	u32;
	//typedef     int				s32;
	typedef		float			f32;
	typedef		double			f64;
	typedef		unsigned char	ubyte;



	typedef     char			sc8;
	//typedef		short			s16;
	typedef		unsigned char	uc8;
	//typedef     unsigned short	u16;



#define KB		(1024)
#define MB		(KB * KB)
#define GB		(KB * KB * KB)
#define MaxInt  (0x7FFFFFFF)
#define MinInt  (-MaxInt - 1)

#define MaxUInt32 (0xFFFFFFFFu)
	typedef rapidjson::Document				JsonDocument;
	typedef JsonDocument::ValueType			JsonObject;

}
