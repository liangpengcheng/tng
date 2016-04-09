#include "graphics/graphics.h"
#include "graphics/scene.h"
#include "graphics/camera.h"
#include "graphics/renderservice.h"
#include "engine/engine.h"
#include "resource/resource_manger.h"
namespace tng
{
	namespace graphics
	{
		void InitGraphics()
		{
			bool inited = false;
			if (!inited)
			{
				engine::InitEngine();
				ResourceManger::InitStatic();
				GFXService::InitStatic();
				RenderService::InitStatic();
				WindowManger::InitStatic();
				inited = true;
			}
		}
	}
}