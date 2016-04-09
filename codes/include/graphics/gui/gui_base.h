#ifndef _TNG_GUI_BASE_H_
#define _TNG_GUI_BASE_H_
#include "../scene2d.h"
namespace tng
{
	///base of gui node
	class GuiBase :public SceneNode2d
	{
	public:
		//properties
		__inline s32 GetWidth()const{ return width_; }
		__inline s32 GetHight()const{ return hight_; }
		__inline void SetWidth(s32 v){ width_ = v; }
		__inline void SetHight(s32 v){ hight_ = v; }
		//end properties


		ENGINE_CLASS_DEF
	protected:
		s32		width_, hight_;

	};
}
#endif