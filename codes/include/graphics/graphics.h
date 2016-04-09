#pragma once
#include "graphics_macros.h"
#include "engine/service.h"
#include "window.h"
#include "graphics/render.h"
#include "graphics/scenemanger.h"
#include "resource/resource_manger.h"
#include "graphics/material_compiler.h"
namespace tng
{
	class GFX_API GFXService:public UpdateableService
	{
	public:
		ENGINE_CLASS_DEF
		GFXService();
		~GFXService();
		static GFXService*		CreateGraphics();
		static GFXService*		GetInstance(){return global_gfx_;}
		
		bool					Initialize(Loop* loop);
		void					OnCreate();
		void					CreateMainRenderWindow(const string& appname,u32 width, u32 height,u32 style);
#ifndef ANDROID
		WindowManger*			GetWindowManger(){return window_manger_;}
#endif
		void					SetRender(Render* r);
		Render*					GetRender(){return render_;}
		TngWindow*				GetMainWindow(){return main_window;}
		SceneManger*			GetSceneManger(){return scene_manger_;}
		MaterialManger*			GetMaterialManger(){return mtl_mgr_;}
		class RenderService*	GetRenderService(){return render_service_;}
		void					Exit();
	protected:
		virtual void			_update(f32 deltaTime);
	private:
		static GFXService*		global_gfx_;
#ifndef ANDROID
		WindowManger*			window_manger_;
#endif
		class RenderService*	render_service_;
		TngWindow*				main_window;
		SceneManger*			scene_manger_;
		Render*					render_;
		ResourceManger*			res_mgr_;
		MaterialManger*			mtl_mgr_;


	};
	namespace graphics
	{
		GFX_FUNC_API void InitGraphics();
	}
	namespace internal
	{
		__inline Render*	GetRender()
		{
			if(!GFXService::GetInstance())
				return NULL;
			return GFXService::GetInstance()->GetRender();
		}
		__inline class RenderService*	GetRenderService()
		{
			if(!GFXService::GetInstance())
				return NULL;
			return GFXService::GetInstance()->GetRenderService();
		}
	}
}