#ifndef _TNG_GLES_TEXTURE_H_
#define _TNG_GLES_TEXTURE_H_
#include "glesrender/gles_render_macros.h"
#include "graphics/rendertexture.h"
namespace tng
{
	class GLESTexture:public RenderTexture
	{
	public:
		GLESTexture();
		virtual void			CreateTexture(TextureData* data);
	public:
		GLuint							texture_;
		char*							lock_stream_;
		s32								lock_length_;
	};
}
#endif