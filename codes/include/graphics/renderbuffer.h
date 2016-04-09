#ifndef  _tng_renderbuffer_h__
#define  _tng_renderbuffer_h__
#include "core/types.h"
#include "core/refobject.h"
#include "graphics_macros.h"

namespace tng
{
	// mapping types
	enum LockType
	{
		LockRead,                // gain read access, must be UsageDynamic and AccessRead
		LockWrite,               // gain write access, must be UsageDynamic and AccessWrite
		LockReadWrite,           // gain read/write access, must be UsageDynamic and AccessReadWrite
		LockWriteDiscard,        // gain write access, discard previous content, must be UsageDynamic and AccessWrite
		LockWriteNoOverwrite,    // gain write access, must be UsageDynamic and AccessWrite, see D3D10 docs for details
	};
	
	class LockInfo
	{
	public:
		LockInfo() : data(0), rowPitch(0), depthPitch(0) {};

		void* data;
		u32 rowPitch;
		u32 depthPitch;
	};

	class GFX_API RenderBuffer:public RefCountedObject
	{
	public:
	};
}
#endif