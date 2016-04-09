#include "graphics/gui/gui_base.h"

namespace tng
{
	START_SUB_CLASS(GuiBase, SceneNode2d)
		Property(GuiBase, s32, "Width", GetWidth, SetWidth)
		Property(GuiBase, s32, "Hight", GetHight, SetHight)
	END_CLASS
}