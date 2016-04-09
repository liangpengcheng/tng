#include "core/arch_detect.h"
#ifdef TNG_ANDROID
#include "graphics/graphics.h"
#include "graphics/window.h"
#include "core/runtime_check.h"
#include "engine/engine.h"
#include "graphics/renderservice.h"
namespace tng{


void TngWindow::engine_handle_cmd(struct android_app* app, int32_t cmd)
{
	switch (cmd)
	{
	case APP_CMD_INIT_WINDOW:
	{
		if(app->window!=NULL)
		{
			OS::state__ = app;
			Log::GetLog()->Printf(Log::SYS_CHN,"init window");
			RenderService::init_render_ = 0;
		}
	}	
		break;
	case APP_CMD_TERM_WINDOW:
		Log::GetLog()->Printf(Log::SYS_CHN,"term window");
		RenderService::destroy_surface_ = 1;
		break;
	case APP_CMD_DESTROY:
		Log::GetLog()->Printf(Log::SYS_CHN,"destroy window");
		break;
	case APP_CMD_GAINED_FOCUS:
		Log::GetLog()->Printf(Log::SYS_CHN,"window gain focus");
		break;
	case APP_CMD_RESUME:
		Engine::GetInstance()->OnResume();
		break;
	case APP_CMD_PAUSE:
		Engine::GetInstance()->OnPause();
		break;
	case APP_CMD_LOST_FOCUS:
		Log::GetLog()->Printf(Log::SYS_CHN,"window lost focus");
		break;
	}
}
WINHANDLE TngWindow::Create(const string& appname, u32 width, u32 height, u32 style)
{
	return NULL;
}
int32_t TngWindow::engine_handle_input(struct android_app* app, AInputEvent* event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		s32 action = AMotionEvent_getAction(event);
		size_t pointerIndex;
		size_t pointerId;
		size_t pointerCount;
		int x;
		int y;

		switch (action & AMOTION_EVENT_ACTION_MASK)
		{
		case AMOTION_EVENT_ACTION_DOWN:
		{
										  pointerId = AMotionEvent_getPointerId(event, 0);
										  float2 pos;
										  pos.x = (float)AMotionEvent_getXOffset(event);
										  pos.y = (float)AMotionEvent_getYOffset(event);

										  InputEvent* input = new InputEvent(InputBasePtr(new InputDown(pos, ELButton)));
										  input->input_->input_index_ = pointerId;
										  SendEvent(input, NULL);

										  break;
		}
		case AMOTION_EVENT_ACTION_UP:
		{
										pointerId = AMotionEvent_getPointerId(event, 0);
										float2 pos;
										pos.x = (float)AMotionEvent_getXOffset(event);
										pos.y = (float)AMotionEvent_getYOffset(event);

										InputEvent* input = new InputEvent(InputBasePtr(new InputClick(pos, ELButton)));
										input->input_->input_index_ = pointerId;
										SendEvent(input, NULL);
										break;
		}
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
		{
												  pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
												  pointerId = AMotionEvent_getPointerId(event, pointerIndex);
												  float2 pos;
												  pos.x = (float)AMotionEvent_getX(event, pointerIndex);
												  pos.y = (float)AMotionEvent_getY(event, pointerIndex);

												  InputEvent* input = new InputEvent(InputBasePtr(new InputDown(pos, ELButton)));
												  input->input_->input_index_ = pointerId;
												  SendEvent(input, NULL);

												  break;
		}
		case AMOTION_EVENT_ACTION_POINTER_UP:
		{
												pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
												pointerId = AMotionEvent_getPointerId(event, pointerIndex);
												float2 pos;
												pos.x = (float)AMotionEvent_getX(event, pointerIndex);
												pos.y = (float)AMotionEvent_getY(event, pointerIndex);

												InputEvent* input = new InputEvent(InputBasePtr(new InputClick(pos, ELButton)));
												input->input_->input_index_ = pointerId;
												SendEvent(input, NULL);
												break;
		}
		case AMOTION_EVENT_ACTION_MOVE:
		{
										  pointerCount = AMotionEvent_getPointerCount(event);
										  for (size_t i = 0; i < pointerCount; ++i)
										  {
											  pointerId = AMotionEvent_getPointerId(event, i);
											  float2 pos;
											  pos.x = AMotionEvent_getX(event, i);
											  pos.y = AMotionEvent_getY(event, i);

											  InputEvent* input = new InputEvent(InputBasePtr(new InputMove(pos)));
											  input->input_->input_index_ = pointerId;
											  SendEvent(input, NULL);

										  }
		}
			break;
		}
		return 1;
	}
	else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
	{
		s32 action = AKeyEvent_getAction(event);
		s32 keycode = AKeyEvent_getKeyCode(event);
		s32 metastate = AKeyEvent_getMetaState(event);

		//Don't consume volume up/down events.
		if (keycode == AKEYCODE_VOLUME_DOWN || keycode == AKEYCODE_VOLUME_UP)
			return 0;

		switch (action)
		{
		case AKEY_EVENT_ACTION_DOWN:

			break;

		case AKEY_EVENT_ACTION_UP:

			break;
		}

		return 1;
	}
	return 0;
}
}
#endif // TNG_ANDROID
