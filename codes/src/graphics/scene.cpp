#include "graphics/scene.h"
#include "graphics/graphics.h"
#include "graphics/rendercontex.h"
#include "graphics/scenerender.h"
#include "graphics/renderservice.h"
namespace tng
{
	Scene::Scene() :input_state_(0)
	{
		render_ = GFXService::GetInstance()->GetRender()->CreateSceneRender();
	}
	Scene::~Scene(){}
	void Scene::SetDisplay(struct RenderContex* rc)
	{
		Assert(render_);
		render_->rendercontex_ = rc;
		Connect(rc->window_, EVENT_INPUT);
	}
	void Scene::SetMainDisplay()
	{
		RenderContex* rcw = new RenderContex();
		rcw->window_ =GFXService::GetInstance()->GetMainWindow();
		SetDisplay(rcw);
	}
	void Scene::SyncToRenderService()
	{
		//collect scene data to scenerender
		_collectDataToRenderService(render_);
	}
	Scene* SceneNode::FindScene()
	{
		if (father_)
		{
			if (!FatherSceneNode()->FindScene())
			{
				return father_->DynamicCast<Scene>();
			}
			else 
				return NULL;
		}
		else
		{
			return NULL;
		}
	}
	void Scene::Update(f32 delta_time)
	{
		SceneNode::Update(delta_time);
		if (render_&&render_->rendercontex_&&render_->rendercontex_->window_)
			render_->rendercontex_->window_->Update(delta_time);// update dispatcher
	}

	void Scene::OnMouseDown(const float2& pos, EInputState state, s32 input_index)
	{
		Log::GetLog()->Printf(Log::DBG_CHN, "Touch down :x(%f) y(%f)", pos.x, pos.y);
		input_state_ |= state;
	}
	void Scene::OnMouseMove(const float2& dir, s32 input_index){
		Log::GetLog()->Printf(Log::DBG_CHN, "Touch move :x(%f) y(%f)", dir.x, dir.y);
		if (input_state_!=0)
		{
			OnDrag(dir, input_index);
		}
	}
	void Scene::OnDrag(const float2& dir, s32 input_index){
		
	}
	void Scene::OnClick(const float2& pos, EInputState state, s32 input_index)
	{
		Log::GetLog()->Printf(Log::DBG_CHN, "Touch click :x(%f) y(%f)", pos.x, pos.y);
		input_state_ &= ~state;
	}
}