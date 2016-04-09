#ifndef _renderindexbuffer__h__
#define _renderindexbuffer__h__
#include "renderbuffer.h"
namespace tng
{
	class GFX_API IndexBuffer :public RenderBuffer
	{
	public:
		IndexBuffer() :dynamic_(false),count_(0){}
		virtual void			UpdateBuffer(u32 offset,u32 size,void** ppData) =0;
		u32						count_;
		bool					dynamic_;
	};
	typedef AutoPtr<IndexBuffer>	IndexBufferPtr;
}

#endif