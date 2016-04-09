#ifndef _GLES_INDEXBUFFER_H_
#define _GLES_INDEXBUFFER_H_
#include "graphics/renderindexbuffer.h"
#include "glesrender/gles_render_macros.h"
namespace tng
{
	class GLESIndexBuffer:public IndexBuffer
	{
	public:
		~GLESIndexBuffer()
		{
			glDeleteBuffers(1, &indexbuffer_);
		}
		virtual void			UpdateBuffer(u32 offset,u32 size,void** ppData){
			glBindBuffer(GL_ARRAY_BUFFER, indexbuffer_);

			GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, offset, size, ppData) );
		}
		GLuint indexbuffer_;
	};
}
#endif