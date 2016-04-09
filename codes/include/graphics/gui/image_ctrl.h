#ifndef _TNG_DIALOG_H_
#define _TNG_DIALOG_H_
#include "gui_base.h"
namespace tng{
	class ImageCtrl :public GuiBase
	{
	public:
		ENGINE_CLASS_DEF
	public:
		void					_collectDataToRenderService(class SceneRender* sr);
		void					SetUV(const float2& uv){ uv_ = uv; }
		float2					GetUV()const{ return uv_; }
	protected:
		TexturePtr		texture_;
		float2			uv_;
	};
}
#endif