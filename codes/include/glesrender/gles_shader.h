#ifndef _TNG_GLES_SHADER_H_
#define _TNG_GLES_SHADER_H_
#include "gles_render_macros.h"
#include "graphics/shader.h"

namespace tng
{

	class GLES_API GLESEffect:public Shader
	{
	public:
		~GLESEffect()
		{
			GL_ASSERT( glDeleteProgram(program_) );
		}

		GLuint program_;
	}; 


}

#endif