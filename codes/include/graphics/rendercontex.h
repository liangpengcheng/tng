#ifndef _TNG_RENDERCONTEX__H_
#define _TNG_RENDERCONTEX__H_
#include "core/arch_detect.h"
#include "window.h"
#include "graphics/rendertexture.h"
#include "graphics/render.h"
namespace tng
{
	struct GFX_API RenderContex:public RefCountedObject
	{
		RenderContex() :window_(NULL) {}
		void SetViewPort(Render* r);


		TngWindow*			window_;
		virtual u32 GetWidth()const 
		{
#ifdef TNG_ANDROID
			return window_->width_;

#else
			Rect rc = window_->GetRect();
			return rc.right - rc.left;
#endif // TNG_ANDROID

		}
		virtual u32 GetHeight()const {
#ifdef TNG_ANDROID
			return window_->height_;

#else
			Rect rc = window_->GetRect();
			return rc.bottom - rc.top;
#endif // TNG_ANDROID

		}
		virtual void OnFinish(Render* r);

	};

	typedef AutoPtr<RenderContex>	RenderContexPtr;

	struct GFX_API RenderContexRenderTarget:public RenderContex
	{
		virtual void OnFinish(Render* r){
			Assert(0);
		}
		virtual u32 GetWidth()const{
			Assert(0);
			return 0;
		}
		virtual u32 GetHeight()const{
			Assert(0);
			return 0;
		}
	};
}
#endif