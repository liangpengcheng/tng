#include "graphics/scenenode2d.h"
#include "graphics/model.h"
namespace tng
{
	SceneNode2d::SceneNode2d()
	{

	}
	void SceneNode2d::UpdateDirty()
	{
		if (dirty_transform_)
		{
			dirty_transform_ = false;
			///
			for (int i = 0 ;i < model_.GetMeshCount(); i++)
			{
				MeshPtr m = model_.GetMesh(i);
				//m->memory_mesh_
				//if(failed)
				//	dirty_transform_ = true;
			}
		}
	}

	START_SUB_CLASS(SceneNode2d, LinkedNode)
	END_CLASS
	
}
