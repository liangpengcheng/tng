#include "graphics/scene2d.h"

namespace tng
{
	Scene2D::Scene2D():min_size_(800,600),max_size_(2600,1920){

	}
	float2 Scene2D::GetFixedSize(const float2& insize)const
	{
		float2 fixed_size = insize;
		if (min_size_.Dot(min_size_) > floatMax)
		{
			if (current_contex_size_.x<min_size_.x)
			{
				fixed_size.x = insize.x*current_contex_size_.x/min_size_.x;
			}
			if (current_contex_size_.y<min_size_.y)
			{
				fixed_size.y = insize.y*current_contex_size_.y/min_size_.y;
			}
		}
		if (max_size_.Dot(max_size_) > floatMax)
		{
			if (current_contex_size_.x>max_size_.x)
			{
				fixed_size.x = insize.x*current_contex_size_.x/max_size_.x;
			}
			if (current_contex_size_.y>max_size_.y)
			{
				fixed_size.y = insize.y*current_contex_size_.y/max_size_.y;
			}
		}
		return fixed_size;
	}
}