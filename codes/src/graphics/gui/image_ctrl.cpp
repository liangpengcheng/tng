#include "graphics/gui/image_ctrl.h"
namespace tng{
	void ImageCtrl::_collectDataToRenderService(class SceneRender* sr)
	{

	}

	START_SUB_CLASS(ImageCtrl, GuiBase)
		Property(ImageCtrl,float2, "UV", GetUV, SetUV)
	END_CLASS
}