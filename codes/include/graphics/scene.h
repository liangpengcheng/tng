#ifndef __tng_scene_h__
#define __tng_scene_h__
#include <string>
#include <iostream>

#include "core/object.h"
#include "graphics_macros.h"
#include "scenenode.h"
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

		virtual	void					Update(f32 delta_time);
	protected:
		
		virtual void	OnEvent(Event* pEvent);
		class SceneRender*		render_;

		u32 input_state_;
	
	};
	typedef AutoPtr<Scene>		ScenePtr;
}

#endif