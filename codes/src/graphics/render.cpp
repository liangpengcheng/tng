#include "graphics/render.h"
#include "graphics/scenerender.h"
#include "core/log.h"

namespace tng
{
	void RenderConfig::Load(JsonObject& doc)
	{
		back_buffer_sizex_	= (u32)doc["bufferx"].GetInt();
		back_buffer_sizey_	= (u32)doc["buffery"].GetInt();
		colorsize_			= (u32)doc["color"].GetInt();
		depthsize_			= (u32)doc["depth"].GetInt();
		stencilsize_		= (u32)doc["stencil"].GetInt();
		multisampling_		= (u32)doc["multisampling"].GetInt();
		vsync_				= doc["vsync"].GetBool();
		earlyZ_				= doc["earlyz"].GetBool();
	}
	Render::Render() :clearmask_(0)
		,current_shader_(NULL)
		,current_mesh_(NULL)
	{
		for (s32 i = 0 ;i < MAX_TEXTURE;i++)
		{
			current_texture_[i] =NULL;
		}
	}
	

	void Render::Initialize(JsonObject& doc)
	{
		RenderConfig::GetInstance().Load(doc);
		Log::GetLog()->Printf(Log::SYS_CHN,"render initialized");

		

	}

	SceneRender*	Render::CreateSceneRender()
	{
		return new SceneRender;
	}

	void Render::InitializeDefaultState()
	{
		_setZEnable(true);
		_setZWriteEnable(true);
// 		Rect rect;
// 		rect.left = 0;
// 		rect.right = renderconfig_.back_buffer_sizex_;
// 		rect.top =0;
// 		rect.bottom = renderconfig_.back_buffer_sizey_;
// 		SetViewport(rect);
		
		SetDepthFun(Depth_LessEqual);
		SetDepthRange(0.0f,1.0f);
	}
	
}