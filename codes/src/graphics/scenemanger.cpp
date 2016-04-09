#include "graphics/scenemanger.h"
#include "graphics/graphics.h"
#include "graphics/renderservice.h"
#include "graphics/scenenode.h"
namespace tng
{
	SceneManger::SceneManger()
	{
		frame_counter_ = 0;
	}
	SceneManger::~SceneManger()
	{

	}
	Scene* SceneManger::CreateEmptyScene(const string& scenename)
	{
		if (scene_map_.find(scenename)!=scene_map_.end())
		{
			throw ApplicationException("Engine exception","scene has been exist");
		}else
		{
			Scene* scene = new Scene;
			scene->SetName(scenename);
			scene_map_[scenename] = scene;
			return scene;
		}
		return NULL;
	}

	void SceneManger::Update(f32 delta_time)
	{
		if (Engine::GetInstance()->IsRunning())
		{
			for (SceneMap::iterator it = scene_map_.begin();
				it!= scene_map_.end();
				it++)
			{
				(*it).second->Update(delta_time);
			}

			GFXService::GetInstance()->GetRenderService()->LockRenderService(true);
			for (SceneMap::iterator it = scene_map_.begin();
				it!= scene_map_.end();
				it++)
			{
				GFXService::GetInstance()->GetRenderService()->BeginRenderScene((*it).second);
			}
			frame_counter_++;
			GFXService::GetInstance()->GetRenderService()->UnlockRenderService();
#ifndef NO_RENDER_THREAD
			GFXService::GetInstance()->GetRenderService()->NotifyMe();
#endif
		}
		
	}

}