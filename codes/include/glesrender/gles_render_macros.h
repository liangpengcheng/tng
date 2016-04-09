#pragma once
#include "graphics/render.h"
#ifdef _WIN32
#ifdef plugin_gles_EXPORTS
#define GLES_API		__declspec(dllexport)
#define GLES_FUNC_API	extern "C" __declspec( dllexport )
#define GLES_GLOBAL_API	extern __declspec( dllexport )
#elif defined(_LIB)
#define GLES_API
#define GLES_FUNC_API	extern "C"
#define GLES_GLOBAL_API 	extern
#else
#define GLES_API		__declspec( dllimport )
#define GLES_FUNC_API	extern "C" __declspec( dllimport )
#define GLES_GLOBAL_API	extern __declspec( dllimport )
#endif// 
#else
#ifdef plugin_gles_EXPORTS
#define GLES_API			__attribute__ ((visibility("default")))
#define GLES_FUNC_API		__attribute__ ((visibility("default")))
#define GLES_GLOBAL_API		__attribute__ ((visibility("default")))
#elif defined(_LIB)
#define GLES_API
#define GLES_FUNC_API	extern "C"
#define GLES_GLOBAL_API 	extern
#else
#define GLES_API
#define GLES_FUNC_API
#define GLES_GLOBAL_API
#endif
#endif//  


#ifdef TNG_IPHONE

#define USE_ES_VERSION 1
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined (__APPLE__)
#include "glew/glew.h"
#include "glew/wglew.h"
#elif TNG_OS_FAMILY_WINDOWS

#define GLEW_STATIC
#include "glew/glew.h"
#include "glew/wglew.h"
#elif defined(TNG_OS_FLASCC)
#include "gl/gl.h"
#elif defined(ANDROID)
#define USE_ES_VERSION 2
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "EGL/egl.h"

extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;

#endif
#ifdef HASXGL
#define GLEW_STATIC
#include "GL/glew.h"

#endif
#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

#ifndef GL_DEPTH24_STENCIL8
#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#endif

#ifndef GL_WRITE_ONLY   
#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#endif



#ifndef glMapBuffer      
#define glMapBuffer glMapBufferOES
#endif

#ifndef glUnmapBuffer      
#define glUnmapBuffer glUnmapBufferOES
#endif

#ifdef NDEBUG
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) do \
	{ \
	gl_code; \
	GLenum errorcode = glGetError();\
	Assert(errorcode== GL_NO_ERROR); \
	} while(0)
#endif

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

namespace tng
{
	__inline const char* GetVertexAttr(VertexType type,u32 index)
	{
		switch (type)
		{
		case tng::VTYPE_UNKONWN:
			return "unkonwn";
			break;
		case tng::VTYPE_GENERIC:
			return "unkonwn";
			break;
		case tng::VTYPE_VERTEX:
			if (index==1)
			{
				return "a_position1";
			}
			else
				return "a_position";
			break;
		case tng::VTYPE_NORMAL:
			return "a_normal";
			break;
		case tng::VTYPE_BINORMAL:
			return "a_binormal";
			break;
		case tng::VTYPE_TANGENT:
			return "a_tangent";
			break;
		case tng::VTYPE_TEXCOORD:
			if(index==1)
				return "a_texcoord1";
			else
				return "a_texcoord";
			break;
		case tng::VTYPE_COLOR:
			return "a_color";
			break;
		case tng::VTYPE_BLEND:
			return "a_blend";
			break;
		case tng::VTYPE_WEIGHT:
			return "a_weight";
			break;
		default:
			return "unknown";
			break;
		}
	}
__inline u32 GetGLType(RenderType rt)
{
	switch (rt)
	{
	case RT_UNSUPPORT:
		return 0;
		break;
	case RT_INT:
	case RT_INT2:
	case RT_INT3:
	case RT_INT4:
		return GL_INT;
		break;
	case RT_FLOAT:
	case RT_FLOAT2:
	case RT_FLOAT3:
	case RT_FLOAT4:
	case RT_FLOAT2x2:
	case RT_FLOAT3x3:
	case RT_FLOAT4x4:
		return GL_FLOAT;
		break;
	case RT_BYTE:
	case RT_BYTE2:
	case RT_BYTE3:
	case RT_BYTE4:
		return GL_UNSIGNED_BYTE;
		break;
	case RT_SAMPLER1D:
	case RT_SAMPLER2D:
	case RT_SAMPLER3D:
	case RT_SAMPLERCubic:
		return GL_TEXTURE;
		break;
	default:
		{Assert(0);}
		return 0;
		break;
	}
	{Assert(0);}
	return 0;
}
__inline RenderType GetGLRT(GLenum glt)
{
	switch (glt)
	{
	case GL_INT:
		return RT_INT;
		break;
	case GL_FLOAT:
		return RT_FLOAT;
		break;
	case GL_INT_VEC2:
		return RT_INT2;
		break;
	case GL_INT_VEC3:
		return RT_INT3;
		break;
	case GL_INT_VEC4:
		return RT_INT4;
		break;
	case GL_FLOAT_VEC2:
		return RT_FLOAT2;
		break;
	case GL_FLOAT_VEC3:
		return RT_FLOAT3;
		break;
	case GL_FLOAT_VEC4:
		return RT_FLOAT4;
		break;
	case GL_FLOAT_MAT2:
		return RT_FLOAT2x2;
		break;
	case GL_FLOAT_MAT3:
		return RT_FLOAT3x3;
		break;
	case GL_FLOAT_MAT4:
		return RT_FLOAT4x4;
		break;
#ifndef USE_ES_VERSION
	case GL_SAMPLER_1D:
		return RT_SAMPLER1D;
		break;
#endif // USE_ES_VERSION
		case GL_SAMPLER_2D:
		return RT_SAMPLER2D;
		break;
#ifndef USE_ES_VERSION
	case GL_SAMPLER_3D:
		return RT_SAMPLER3D;
		break;
#endif
	case GL_SAMPLER_CUBE:
		return RT_SAMPLERCubic;
		break;
	default:
		return RT_UNSUPPORT;
		break;
	}
	return RT_UNSUPPORT;
}

__inline GLenum GetGLPixelFormat(PixelFormat::Code p)
{
	switch (p)
	{
	case PixelFormat::B8G8R8X8:
	case PixelFormat::B8G8R8A8:         return GL_BGRA;
	case PixelFormat::DXT1:				
	case PixelFormat::DXT3:				
	case PixelFormat::DXT5:				
	case PixelFormat::X8R8G8B8:
	case PixelFormat::A8R8G8B8:         return GL_RGBA;
	case PixelFormat::R8G8B8:			return GL_RGB;
	case PixelFormat::A8:				return GL_ALPHA;
	case PixelFormat::L8A8:				return GL_LUMINANCE_ALPHA;
	case PixelFormat::ETC1_RGB8:
		//#ifdef __OSX__
		Log::GetLog()->Printf(Log::WARNING_CHN, "Unspported pixel format\n" );
		return GL_RGBA;
		// #else
		// 		return GL_ETC1_RGB8_OES;
		// #endif
#ifdef GL_IMG_texture_compression_pvrtc
	case PixelFormat::PVRTC_RGB2:        return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
	case PixelFormat::PVRTC_RGBA2:       return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	case PixelFormat::PVRTC_RGB4:        return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
	case PixelFormat::PVRTC_RGBA4:       return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
#else
	case PixelFormat::PVRTC_RGB2:       
	case PixelFormat::PVRTC_RGBA2:      
	case PixelFormat::PVRTC_RGB4:       
	case PixelFormat::PVRTC_RGBA4:
		Log::GetLog()->Printf(Log::WARNING_CHN," PVRTC texture is not supported on this GLES");
		return GL_RGBA;
#endif

	default:   
		Log::GetLog()->Printf(Log::WARNING_CHN," Unknown Pixel Format"); 
		return GL_RGBA;
	}
}
__inline GLenum GetGLInternelFormat(PixelFormat::Code p)
{
	switch (p)
	{
#ifndef USE_ES_VERSION
	case PixelFormat::DXT1:				return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
	case PixelFormat::DXT3:				return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
	case PixelFormat::DXT5:				return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
#endif
	case PixelFormat::B8G8R8X8:
	case PixelFormat::B8G8R8A8:
	case PixelFormat::X8R8G8B8:
	case PixelFormat::A8R8G8B8:
#ifndef USE_ES_VERSION
		return GL_RGBA8;
#else
		return GL_RGBA;
#endif
	case PixelFormat::R8G8B8:			
#ifndef USE_ES_VERSION
		return GL_RGB8;
	case PixelFormat::A8:				return GL_ALPHA8;
	case PixelFormat::L8A8:				return GL_LUMINANCE8_ALPHA8;
#else
		return GL_RGB;
	case PixelFormat::ETC1_RGB8:
		#ifdef __OSX__
		Log::GetLog()->Printf(Log::WARNING_CHN, "Unspported pixel format\n" );
		return GL_RGBA;
		#else
				return GL_ETC1_RGB8_OES;
		#endif
#ifdef GL_IMG_texture_compression_pvrtc
	case PixelFormat::PVRTC_RGB2:        return GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
	case PixelFormat::PVRTC_RGBA2:       return GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	case PixelFormat::PVRTC_RGB4:        return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
	case PixelFormat::PVRTC_RGBA4:       return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
#else
	case PixelFormat::PVRTC_RGB2:       
	case PixelFormat::PVRTC_RGBA2:      
	case PixelFormat::PVRTC_RGB4:       
	case PixelFormat::PVRTC_RGBA4:
		Log::GetLog()->Printf(Log::WARNING_CHN," PVRTC texture is not supported on this GLES");
		return GL_RGBA;
#endif
#endif
	default:   
		Log::GetLog()->Printf(Log::WARNING_CHN," Unknown Pixel Format"); 
		return GL_RGBA;
	}

}

__inline GLenum GetGLTextureType(TextureType t)
{
	switch(t)
	{
	case Tex_2D:
		return GL_TEXTURE_2D;
	case Tex_Cube:
		return GL_TEXTURE_CUBE_MAP;
	default:
		Log::GetLog()->Printf(Log::WARNING_CHN,"GLESTypes::AsGLESTextureType: Invalid Type\n");
		return GL_TEXTURE_2D;
	}
}

__inline GLenum GetGLESPixelDataType(PixelFormat::Code p)
{
	switch (p)
	{
	case PixelFormat::L8A8:
	case PixelFormat::A8:
	case PixelFormat::X8R8G8B8:
	case PixelFormat::A8R8G8B8:         
	case PixelFormat::R8G8B8:			return GL_UNSIGNED_BYTE;
	case PixelFormat::ETC1_RGB8:
		Log::GetLog()->Printf(Log::WARNING_CHN, "Unspported pixel format\n" );
		return GL_UNSIGNED_BYTE;

#ifdef GL_IMG_texture_compression_pvrtc
	case PixelFormat::PVRTC_RGB2:        return GL_UNSIGNED_BYTE;
	case PixelFormat::PVRTC_RGBA2:       return GL_UNSIGNED_BYTE;
	case PixelFormat::PVRTC_RGB4:        return GL_UNSIGNED_BYTE;
	case PixelFormat::PVRTC_RGBA4:       return GL_UNSIGNED_BYTE;
#else
	case PixelFormat::PVRTC_RGB2:       
	case PixelFormat::PVRTC_RGBA2:      
	case PixelFormat::PVRTC_RGB4:       
	case PixelFormat::PVRTC_RGBA4:
		Log::GetLog()->Printf(Log::WARNING_CHN,"PVRTC texture is not supported on this GLES");
		return GL_UNSIGNED_BYTE;
#endif


	default:
		Log::GetLog()->Printf(Log::WARNING_CHN,"Unknown Pixel Format");
		return GL_UNSIGNED_BYTE;
	}
}
}