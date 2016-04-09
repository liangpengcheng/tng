
#include "graphics/window.h"
#ifdef HASXGL
#include "GL/glx.h"
#endif
#include "core/log.h"
#include "engine/engine.h"
#include "graphics/graphics.h"
#include "graphics/renderservice.h"

namespace tng
{
	//TngWindow::WinMap TngWindow::winmap_;
	TngWindow::TngWindow() :main_(false)
	{}

	TngWindow::~TngWindow()
	{

		if (IsMain())
		{
			GFXService::GetInstance()->Exit();
		}
	}

	Rect	TngWindow::GetRect()
	{
		RECT rc;
		return rc;
	}
	


	WindowManger* WindowManger::global_window_manger_ = NULL;
	WindowManger::WindowManger()
	{
		if (global_window_manger_)
			throw ApplicationException("Engine exception","Window manger instance has been exist");
		global_window_manger_ = this;

	}
	WindowManger::~WindowManger()
	{
		global_window_manger_ = NULL;

	}
	void WindowManger::CreateWnd(const string& appname,u32 width, u32 height,u32 style)
	{
		CreateWndCommand* command = new CreateWndCommand;
		command->appname = appname;
		command->width = width;
		command->height = height;
		command->style=style;
		winmutex_.lock();
		commands_.push_back(command);
		winmutex_.unlock();
	}
	TngWindow* WindowManger::_createwnd_internel(const string& appname,u32 width, u32 height,u32 style)
	{
		TngWindow* wnd = new TngWindow;
		wnd->Create(appname,width,height,style);
		if (winmap_.find(appname)!=winmap_.end())
		{
			delete wnd;
			throw ApplicationException("Engine exception","window has been exist");
			return NULL;
		}
		else
		{
			Log::GetLog()->Printf(Log::SYS_CHN,"window(%s %dx%d) created",appname.c_str(),width,height);
			winmap_[appname] = wnd;
			return wnd;
		}
	}
	void WindowManger::ShutWindow(const string& appname)
	{
		winmutex_.lock();
		WinMap::iterator it = winmap_.find(appname);
		if (it!=winmap_.end())
		{
			winmutex_.unlock();
#ifdef TNG_OS_FAMILY_WINDOWS
			DestroyWindow(it->second->handle_);
#endif // TNG_OS_FAMILY_WINDOWS
		}
	}
// 	TngWindow* WindowManger::CreateWndIME(const string& appname,u32 width, u32 height,u32 style)
// 	{
// 		AutoLock<Mutex> lt(&winmutex_);
// 		return WindowManger::GetInstance()->_createwnd_internel(appname,width,height,style);
// 	}
	void WindowManger::RemoveWindow(const string& appname)
	{
		AutoLock<Mutex> lt(&winmutex_);
		WinMap::iterator it = winmap_.find(appname);
		if (it!=winmap_.end())
		{
			winmap_.erase(it);
		}
		Log::GetLog()->Printf(Log::SYS_CHN,"window(%s) removed",appname.c_str());
	}
	TngWindow* WindowManger::FindWindow(const string& appname)
	{
		AutoLock<Mutex> lt(&winmutex_);
		WinMap::iterator it = winmap_.find(appname);
		if (it!=winmap_.end())
		{
			return it->second;
		}
		return NULL;
	}
	void WindowManger::_update(f32 deltaTime)
	{
		winmutex_.lock();
		for (std::list<Command*>::iterator it = commands_.begin();
			it!=commands_.end();
			it++)
		{
			(*it)->Do();
		}
		commands_.clear();
		winmutex_.unlock();
#ifdef TNG_OS_FAMILY_WINDOWS
		MSG  msg;
		while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
#endif // TNG_OS_FAMILY_WINDOWS
#ifdef TNG_USE_X11
	    TngWindow::UpdateEvent();
#endif
	}
	bool CreateWndCommand::Do()
	{
		WindowManger::GetInstance()->_createwnd_internel(appname,width,height,style);
		return true;
	}


	START_SUB_CLASS(WindowManger,UpdateableService)
	END_CLASS
