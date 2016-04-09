#ifndef _TNG_COLOR_H__
#define _TNG_COLOR_H__
#include "core/math.h"
#include "core/types.h"
#define TNG_BGRA(b,g,r,a) \
	((u32)((((a)& 0xff) << 24) | (((r)& 0xff) << 16) | (((g)& 0xff) << 8) | ((b)& 0xff)))

#define TNG_BGR(b,g,r) \
	((u32)((((0xff) & 0xff) << 24) | (((r)& 0xff) << 16) | (((g)& 0xff) << 8) | ((b)& 0xff)))

#define COLOR_ALPHA_VALUE(color)	((u8)((color>>24)&0xff))	
#define COLOR_RED_VALUE(color)		((u8)((color>>16)&0xff)) 
#define COLOR_GREEN_VALUE(color)	((u8)((color>>8)&0xff))
#define COLOR_BLUE_VALUE(color)		((u8)((color)&0xff))

///< maps floating point channels (0.f to 1.f range) to D3DCOLOR
#define TNG_FLOAT_DWORD(r,g,b,a) \
	TNG_BGRA((u32)((b)*255.f), (u32)((g)*255.f), (u32)((r)*255.f), (u32)((a)*255.f))
#define TNG_FLOAT_DWORD_BGRA(r,g,b,a) \
	TNG_BGRA((u32)((b)*255.f), (u32)((g)*255.f), (u32)((r)*255.f), (u32)((a)*255.f))

namespace tng
{
	struct Color
	{
		f32 r;
		f32 g;
		f32 b;
		f32 a;

		operator float4()
		{
			return float4(r, g, b, a);
		}

		Color() :r(1.0f), g(1.0f), b(1.0f), a(1.0f)
		{}
		Color(f32 v) :r(v), g(v), b(v), a(v)
		{}
		Color(f32 _r, f32 _g, f32 _b) :r(_r), g(_g), b(_b), a(1.0f)
		{}
		Color(f32 _r, f32 _g, f32 _b, f32 _a) :r(_r), g(_g), b(_b), a(_a)
		{}
		Color(u32 dc)
		{
			r = (f32(COLOR_RED_VALUE(dc))) / 255.0f;
			g = (f32(COLOR_GREEN_VALUE(dc))) / 255.0f;
			b = (f32(COLOR_BLUE_VALUE(dc))) / 255.0f;
			a = (f32(COLOR_ALPHA_VALUE(dc))) / 255.0f;
		}
		__inline bool operator ==(const Color& c)const
		{
			return (r == c.r&&g == c.g&&b == c.b&&a == c.a);
		}

		__inline Color& operator = (u32 dc)
		{
			r = (f32(COLOR_RED_VALUE(dc))) / 255.0f;
			g = (f32(COLOR_GREEN_VALUE(dc))) / 255.0f;
			b = (f32(COLOR_BLUE_VALUE(dc))) / 255.0f;
			a = (f32(COLOR_ALPHA_VALUE(dc))) / 255.0f;
			return *this;
		}

		__inline u32	ToU32()const
		{
			return TNG_FLOAT_DWORD(r, g, b, a);
		}
	};
}

#endif