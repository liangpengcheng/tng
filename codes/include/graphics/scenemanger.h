#ifndef __tng_scenemanger_h__
#define __tng_scenemanger_h__
#include "scene.h"
#include <map>
namespace tng
{
	class GFX_API SceneManger
	{
	public:
		SceneManger();
		~SceneManger();
		typedef unordered_map<string,ScenePtr> SceneMap;
		Scene*									CreateEmptyScene(const string& scenename);
		virtual	void							Update(f32 delta_time);
		u32										GetFrame()
		{
			return u32(frame_counter_);
		}
	protected:
		SceneMap								scene_map_;
#ifndef NO_RENDER_THREAD
		AtomicCounter							frame_counter_;
#else
		u32										frame_counter_;
#endif
		
	};
}

#endif