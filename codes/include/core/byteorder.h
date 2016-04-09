#ifndef _TNG_BYTEORDER_H_
#define _TNG_BYTEORDER_H_
#include "types.h"
namespace tng
{

/**
@brief bytes order swap function
*/
template <typename T>
class Swap_Endian_Fun
{
public:
	//CLASS_NEW(Swap_Endian_Fun<T>);
	__inline void operator()(const T& t, T* val) const
	{
		union {
			T t;
			char chars[8];
		} u;
		char c;
		u.t = t;
		size_t sizeof_T = sizeof(T);
		switch (sizeof_T)
		{
		case 1:
			break;
		case 2:
			c = u.chars[0];
			u.chars[0] = u.chars[1];
			u.chars[1] = c;
			break;
		case 4:
			c = u.chars[0];
			u.chars[0] = u.chars[3];
			u.chars[3] = c;
			c = u.chars[1];
			u.chars[1] = u.chars[2];
			u.chars[2] = c;
			break;
		case 8:
			c = u.chars[0];
			u.chars[0] = u.chars[7];
			u.chars[7] = c;
			c = u.chars[1];
			u.chars[1] = u.chars[6];
			u.chars[6] = c;
			c = u.chars[2];
			u.chars[2] = u.chars[5];
			u.chars[5] = c;
			c = u.chars[3];
			u.chars[3] = u.chars[4];
			u.chars[4] = c;
			break;
		default:
			abort();
			break;
		}
		*(val) = u.t;
	}
};

template <>
class Swap_Endian_Fun<s32>
{
public:
	__inline void operator()(const s32& i, s32* val) const
	{
		*val =
			((i & 0xFF) << 24) |
			((i & 0xFF00) << 8) |
			((i & 0xFF0000) >> 8) |
			((i & 0xFF000000) >> 24);
	}
};

template <>
class Swap_Endian_Fun<u32>
{
public:
	__inline void operator()(const u32& i, u32* val) const
	{
		*val =
			((i & 0xFF) << 24) |
			((i & 0xFF00) << 8) |
			((i & 0xFF0000) >> 8) |
			((i & 0xFF000000) >> 24);
	}
};

template <>
class Swap_Endian_Fun<f32>
{
public:
	__inline void operator()(const f32& f, f32* val) const
	{
		union {
			f32 f;
			s32 i;
		} u;
		u.f = f;
		u.i =
			((u.i & 0xFF) << 24) |
			((u.i & 0xFF00) << 8) |
			((u.i & 0xFF0000) >> 8) |
			((u.i & 0xFF000000) >> 24);
		*val = u.f;
	}
};
}
#endif