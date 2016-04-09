#ifndef _TNG_GUI_RENDER_H_
#define _TNG_GUI_RENDER_H_
#include "scenenode.h"
#include "gui/graphics.hpp"
#include "gui/imageloader.hpp"
#include "gui/image.hpp"
#include "gui/color.hpp"
namespace tng{
	class gui_render :public tng_gui::Graphics
	{
	public:
		virtual void drawImage(const tng_gui::Image* image,
			int srcX,
			int srcY,
			int dstX,
			int dstY,
			int width,
			int height) = 0;

		virtual void drawPoint(int x, int y) = 0;
		virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
		virtual void drawRectangle(int x, int y, int w, int h) = 0;
		virtual void fillRectangle(int x, int y, int w, int h) = 0;
		virtual void setColor(const tng_gui::Color& color) = 0;
		virtual const tng_gui::Color& getColor() const = 0;
	};

	class gui_image:public tng_gui::Image
	{
	public:

		virtual void free(){ texture_ = NULL; }

		virtual int getWidth() const;

		virtual int getHeight() const ;


		virtual tng_gui::Color getPixel(int x, int y);


		//virtual void putPixel(int x, int y, const tng_gui::Color& color) = 0;

		//virtual void convertToDisplayFormat() = 0;

		TexturePtr   texture_;
	};
	class gui_image_loader :public tng_gui::ImageLoader
	{
	public:
		virtual tng_gui::Image* load(const string& filename, bool convertToDisplayFormat = true);
	};
}

#endif