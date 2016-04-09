#ifndef __tng_scene_h__
#define __tng_scene_h__
#include <string>
#include <iostream>

#include "core/object.h"
#include "graphics_macros.h"
#include "scenenode.h"
#include "scene2d.h"
#include "engine/event.h"
namespace tng
{
	

	class GFX_API Scene :public SceneNode, public EventListener
	{
	public:
		
		Scene();
		~Scene();

		void SetDisplay(struct RenderContex* rc);
		void SetMainDisplay();
		void SyncToRenderService();
		void OnMouseDown(const float2& pos, EInputState state,s32 input_index);
		void OnMouseMove(const float2& dir, s32 input_index);
		void OnClick(const float2& pos, EInputState state, s32 input_index);
		void OnDrag(const float2& dir, s32 input_index);
		
		class SceneRender* GetRender(){return render_;}
		Scene2DPtr CreateScene2D()
		{
			scene_2d_ = new Scene2D();
			return scene_2d_;
		}
		Scene2DPtr GetScene2D()
		{
			return scene_2d_;
		}
		const Scene2DPtr GetScene2D()const
		{
			return scene_2d_;
		}
		void SetScene2D(const Scene2DPtr& s){scene_2d_ = s;}
		virtual	void					Update(f32 delta_time);
	protected:
		
		virtual void	OnEvent(Event* pEvent);
		class SceneRender*		render_;
		Scene2DPtr				scene_2d_;

		u32 input_state_;
	
	};
	typedef AutoPtr<Scene>		ScenePtr;
}

#endif