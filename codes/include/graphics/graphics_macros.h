#ifndef __TNG_GFX_MAC_H__
#define __TNG_GFX_MAC_H__
#include "core/arch_detect.h"
#include "core/types.h"
#include "core/runtime_check.h"
#if defined(NO_LIBUV)||defined(EMCC)
#define NO_RENDER_THREAD
#endif
//#define NO_RENDER_THREAD
#ifdef _WIN32
#ifdef graphics_EXPORTS
#define GFX_API		__declspec(dllexport)
#define GFX_FUNC_API	extern "C" __declspec( dllexport )
#define GFX_GLOBAL_API	extern __declspec( dllexport )
#elif defined(_LIB)
#define GFX_API
#define GFX_FUNC_API	extern "C"
#define GFX_GLOBAL_API 	extern
#else
#define GFX_API		__declspec( dllimport )
#define GFX_FUNC_API	extern "C" __declspec( dllimport )
#define GFX_GLOBAL_API	extern __declspec( dllimport )
#endif// 
#else
#ifdef graphics_EXPORTS
#define GFX_API			__attribute__ ((visibility("default")))
#define GFX_FUNC_API		__attribute__ ((visibility("default")))
#define GFX_GLOBAL_API		__attribute__ ((visibility("default")))
#elif defined(_LIB)
#define GFX_API
#define GFX_FUNC_API	extern "C"
#define GFX_GLOBAL_API 	extern
#else
#define GFX_API
#define GFX_FUNC_API
#define GFX_GLOBAL_API
#endif
#endif//  
#define MODEL_FORMAT_VERSION		0x00000001
#define TEXTURE_FORMAT_VERSION		0x00000001

#define USE_VAO 1

namespace tng
{
#if USEGLES
#define DEAFULT_PRECISION	"precision mediump float;\n"
#define SHADER_VERSION			"#version 100\n"
#define LOWP				"lowp"
#else
#define DEAFULT_PRECISION ""
	#define SHADER_VERSION			"#version 120\n"
#define LOWP				""
#endif

enum RenderType
{
	RT_UNSUPPORT,
	RT_INT,
	RT_INT2,
	RT_INT3,
	RT_INT4,
	RT_FLOAT,
	RT_FLOAT2,
	RT_FLOAT3,
	RT_FLOAT4,
	RT_FLOAT2x2,
	RT_FLOAT3x3,
	RT_FLOAT4x4,
	RT_BYTE,
	RT_BYTE2,
	RT_BYTE3,
	RT_BYTE4,
	RT_SAMPLER1D,
	RT_SAMPLER2D,
	RT_SAMPLER3D,
	RT_SAMPLERCubic,
};

__inline u32 GetRenderTypeSize(RenderType rt)
{
	switch (rt)
	{
	case RT_UNSUPPORT:
		return 0;
		break;
	case RT_INT:
		return 4;
		break;
	case RT_INT2:
		return 8;
		break;
	case RT_INT3:
		return 12;
		break;
	case RT_INT4:
		return 16;
		break;
	case RT_FLOAT:
		return 4;
		break;
	case RT_FLOAT2:
		return 8;
		break;
	case RT_FLOAT3:
		return 12;
		break;
	case RT_FLOAT4:
		return 16;
		break;
	case RT_FLOAT2x2:
		return 16;
		break;
	case RT_FLOAT3x3:
		return 36;
		break;
	case RT_FLOAT4x4:
		return 64;
		break;
	case RT_BYTE:
		return 1;
		break;
	case RT_BYTE2:
		return 2;
		break;
	case RT_BYTE3:
		return 3;
		break;
	case RT_BYTE4:
		return 4;
		break;
	case RT_SAMPLER1D:
		{Assert(0);}
		return 0;
		break;
	case RT_SAMPLER2D:
		{Assert(0);}
		return 0;
		break;
	case RT_SAMPLER3D:
		{Assert(0);}
		return 0;
		break;
	case RT_SAMPLERCubic:
		{Assert(0);}
		return 0;
		break;
	default:
		{Assert(0);}
		return 0;
		break;
	}
	{Assert(0);}
	return 0;
}


__inline u32 GetRenderTypeCount(RenderType rt)
{
	switch (rt)
	{
	case RT_UNSUPPORT:
		return 0;
		break;
	case RT_INT:
	case RT_FLOAT:
	case RT_BYTE:
		return 1;
		break;
	case RT_INT2:
	case RT_FLOAT2:
	case RT_BYTE2:
		return 2;
		break;
	case RT_INT3:
	case RT_FLOAT3:
	case RT_BYTE3:
		return 3;
		break;
	case RT_INT4:
	case RT_FLOAT4:
	case RT_BYTE4:
		return 4;
		break;
	default:
		{Assert(0);}
		return 0;
		break;
	}
	{Assert(0);}
	return 0;
}
}

#endif