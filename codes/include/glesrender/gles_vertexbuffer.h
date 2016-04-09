#ifndef _gles_vertexbuffer__h_
#define _gles_vertexbuffer__h_
#include "graphics/rendervertexbuffer.h"
#include "glesrender/gles_render_macros.h"
#include "core/log.h"
namespace tng
{
	class GLESVertexBuffer:public VertexBuffer
	{
	public:
		GLESVertexBuffer() :buffer_(GL_ZERO){}
		~GLESVertexBuffer()
		{
			if(buffer_)
			{
				glDeleteBuffers(1, &buffer_);
				buffer_ = GL_ZERO;
			}
		}
		virtual void			UpdateBuffer(u32 offset,u32 size,void** ppData){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_);

			GL_ASSERT( glBufferSubData(GL_ARRAY_BUFFER, offset, size, ppData) );
		}
		GLuint						buffer_;
	};
	class GLESVAO :public VertexBuffer
	{
	public:
		~GLESVAO(){
			if(buffer_)
			{
				glDeleteVertexArrays(1, &buffer_);
				buffer_ = GL_ZERO;
			}
		}
		virtual void			UpdateBuffer(u32 offset, u32 size, void** ppData){}
		GLuint						buffer_;
	};
}
#endif