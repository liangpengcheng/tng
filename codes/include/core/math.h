#ifndef _TNG_MATH_H__
#define _TNG_MATH_H__
#include "MathGeoLib/Math/MathAll.h"
#include "core/types.h"
#include "core/core_exports.h"
#include "core/memobj.h"
#define THRESH_NORMALS_ARE_SAME			(0.00002f)	

namespace tng
{

	using namespace MATH_NS;

	CORE_GLOBAL_API u32 Crc32(u32 crc, const unsigned char *buf, size_t size);
	__inline int Trunc(float F)		{
		return int(F > 0 ? Floor((F)+0.5f) : Ceil((F)-0.5f));
	}
	struct PackedNormal :public MemObj
	{
		union
		{
			struct
			{
				u8	X,
				Y,
				Z,
				W;

			};
			u32			Packed;
		}				Vec;

		// Constructors.

		PackedNormal() { Vec.Packed = 0; }
		PackedNormal(u32 InPacked) { Vec.Packed = InPacked; }
		PackedNormal(const float3& InVector) { *this = InVector; }

		// Conversion operators.

		void operator=(const float3& InVector)
		{
			Vec.X = Clamp((u8)(Trunc(InVector.x * 127.5f + 127.5f)), u8(0), u8(255));
			Vec.Y = Clamp((u8)(Trunc(InVector.y * 127.5f + 127.5f)), u8(0), u8(255));
			Vec.Z = Clamp((u8)(Trunc(InVector.z * 127.5f + 127.5f)), u8(0), u8(255));
			Vec.W = 0;
		}
		void operator=(const float4& InVector)
		{
			Vec.X = u8(InVector.x);
			Vec.Y = u8(InVector.y);
			Vec.Z = u8(InVector.z);
			Vec.W = u8(InVector.w);
		}
		operator float3() const
		{
			return float3(
				(f32)Vec.X / 127.5f - 1.0f,
				(f32)Vec.Y / 127.5f - 1.0f,
				(f32)Vec.Z / 127.5f - 1.0f
				);
		}
		operator float4()const{
			return float4(Vec.X, Vec.Y, Vec.Z, Vec.W);
		}

		// Equality operator.

		bool operator==(const PackedNormal& B) const
		{
			if (Vec.Packed != B.Vec.Packed)
				return 0;

			float3	V1 = *this,
				V2 = B;

			if (Abs(V1.x - V2.x) > THRESH_NORMALS_ARE_SAME * 4.0f)
				return 0;

			if (Abs(V1.y - V2.y) > THRESH_NORMALS_ARE_SAME * 4.0f)
				return 0;

			if (Abs(V1.z - V2.z) > THRESH_NORMALS_ARE_SAME * 4.0f)
				return 0;

			return 1;
		}

	};
}
#endif