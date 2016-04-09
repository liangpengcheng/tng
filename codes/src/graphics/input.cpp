#include "graphics/input.h"
namespace tng
{
	void Scene::OnEvent(Event* pEvent)
	{
		InputEvent* ievent = (InputEvent*)pEvent;
		if (ievent&&
			ievent->input_)
			ievent->input_->OnInput(this);
	}
}