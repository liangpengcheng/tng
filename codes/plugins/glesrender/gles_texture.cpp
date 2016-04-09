#include "glesrender/gles_texture.h"
#include "graphics/graphics.h"
namespace tng
{
	GLESTexture::GLESTexture() :texture_(GL_ZERO)
	{

	}
	void GLESTexture::CreateTexture(TextureData* data)
	{
		SetTextureHead(data->head_);
		GLenum glinternelformat = GetGLInternelFormat( PixelFormat::Code(head_.fmt_) );
		GL_ASSERT(glBindTexture(GL_TEXTURE_2D, texture_));
		GL_ASSERT(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		head_.mipmaps_ > 0 ? glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR) :
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (PixelFormat::IsCompressed(PixelFormat::Code(head_.fmt_)))
		{
			GLenum gltype   = GetGLTextureType( head_.type_);
			u32 offsetlvl =0;
			for (u32 level = 0;level<head_.mipmaps_+1;level++)
			{
				u32 sz = PixelFormat::GetMemorySize(head_.width_>>level,head_.height_>>level,1,PixelFormat::Code(head_.fmt_));

				GL_ASSERT(glCompressedTexImage2D(
					gltype,
					level,
					glinternelformat,
					head_.width_, head_.height_,
					0,
					sz,
					data->raw_+offsetlvl));
				offsetlvl += sz;
			}
			
		}
		else
		{
			GLenum glformat = GetGLPixelFormat( PixelFormat::Code(head_.fmt_) );
			GLenum gltype = GetGLESPixelDataType(PixelFormat::Code(head_.fmt_));
			GL_ASSERT(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
			u32 offsetlvl =0;
			for (u32 level = 0;level<head_.mipmaps_+1;level++)
			{
				u32 sz = PixelFormat::GetMemorySize(head_.width_>>level,head_.height_>>level,1,PixelFormat::Code(head_.fmt_));
				GL_ASSERT(glTexImage2D(GetGLTextureType( head_.type_), level, glinternelformat, 
					head_.width_, head_.height_,0,
					glformat,
					gltype,
					data->raw_+offsetlvl));
				offsetlvl += sz;
			}
			
			GL_ASSERT(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
		}
	}

}