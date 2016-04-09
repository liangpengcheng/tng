#include "graphics/gui_render.h"

namespace tng{
	tng_gui::Image* gui_image_loader::load(const string& filename, bool convertToDisplayFormat /* = true */)
	{
		gui_image* img = new gui_image();
		FileTexture* ft = new FileTexture();
		ft->LoadResource(filename);
		img->texture_ = ft;
		return NULL;
	}

	int gui_image::getHeight()const{
		if (!texture_.isNull())
		{
			return texture_->head_.height_;
		}
		return -1;
	}
	int gui_image::getWidth()const{
		if (!texture_.isNull())
		{
			return texture_->head_.width_;
		}
		return -1;
	}

	tng_gui::Color gui_image::getPixel(int x, int y)
	{
		Log::GetLog()->Printf(Log::ERROR_CHN,"not support currently");
		return tng_gui::Color(0);
	}




}