#ifndef _rendertexture_h__
#define _rendertexture_h__

#include "core/math.h"
#include "core/autoptr.h"
#include "renderbuffer.h"
#include "pixelformat.h"
#include "texture.h"
namespace tng
{
	
	enum CubeFace
	{
		PosX = 0,
		NegX,
		PosY,
		NegY,
		PosZ,
		NegZ,
	};

	class GFX_API RenderTexture :public RenderBuffer
	{
	public:
		RenderTexture(){}
		virtual void			CreateTexture(TextureData* data) =0;

	public:
		texture_head*		GetTextureHead(){return &head_;}
		void					SetTextureHead(const texture_head& head){head_ = head;}

	protected:
		texture_head	head_;
		//
	};
	typedef AutoPtr<RenderTexture>	TexPtr;
}
#endif