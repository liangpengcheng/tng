#include "graphics/rendercontex.h"
#include "graphics/render.h"
#include "core/arch_detect.h"

namespace tng
{

	void RenderContex::OnFinish(Render* r)
	{
//#ifndef TNG_ANDROID
		Assert(window_);
//#endif
		r->Swap(window_);
	}
	void RenderContex::SetViewPort(Render* r){
		r->SetViewport(Rect(0, 0, GetWidth(), GetHeight()));
	}
}