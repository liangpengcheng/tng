#ifndef __TNG_INPUT_H__
#define __TNG_INPUT_H__

#include "graphics_macros.h"
#include <string>
#include "core/object.h"
#include "core/math.h"
#include "core/smartptr.h"
#include "core/stl_rebind.h"

#include "graphics/scene.h"
namespace tng
{
	
	class InputBase
	{
	public:
		virtual ~InputBase(){}
		InputBase():input_index_(0){}
		virtual void OnInput(Scene* scene) = 0;
		s32 input_index_;
	};
	typedef SmartPtr<InputBase>		InputBasePtr;

	class InputClick :public InputBase
	{
	public:
		InputClick(const float2& pos, EInputState state) :pos_(pos), state_(state){}
		virtual void OnInput(Scene* scene)
		{
			scene->OnClick(pos_, state_, input_index_);
		}
		float2 pos_;
		EInputState state_;
	};
	class InputDown :public InputBase
	{
	public:
		InputDown(const float2& pos, EInputState state) :pos_(pos), state_(state){}
		virtual void OnInput(Scene* scene)
		{
			scene->OnMouseDown(pos_, state_, input_index_);
		}
		float2 pos_;
		EInputState state_;
	};

	class InputMove :public InputBase
	{
	public:
		InputMove(const float2& dir) :dir_(dir){}
		virtual void OnInput(Scene* scene)
		{
			scene->OnMouseMove(dir_, input_index_);
		}
		float2 dir_;

	};
	class InputEvent :public Event
	{
	public:
		InputEvent(InputBasePtr ibase) :input_(ibase){}
		InputBasePtr	input_;
		virtual EventKey GetEventKey(){
			return EVENT_INPUT;
		}
	};
	
}

#endif