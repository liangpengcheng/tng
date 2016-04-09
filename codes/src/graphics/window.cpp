
#include "graphics/window.h"
#ifdef HASXGL
#include "GL/glx.h"
#endif
#include "core/log.h"
#include "engine/engine.h"
#include "graphics/graphics.h"
#include "graphics/renderservice.h"
#if TNG_OS != TNG_OS_MAC_OS_X
namespace tng
{
	//TngWindow::WinMap TngWindow::winmap_;
	TngWindow::TngWindow() :handle_(0),main_(false)
	{}

	TngWindow::~TngWindow()
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		if (handle_)
		{
			::CloseWindow(handle_);
			handle_ = NULL;
		}
#elif defined(TNG_USE_X11)
		XDestroyWindow(WindowManger::GetInstance()->display_,handle_);
#endif
		if (IsMain())
		{
			GFXService::GetInstance()->Exit();
		}
	}

	Rect	TngWindow::GetRect()
	{
		
#ifdef TNG_OS_FAMILY_WINDOWS
		RECT rc;
		::GetClientRect(handle_, &rc);
		//::GetWindowRect(handle_,&rc);
#elif defined(TNG_USE_X11)
		Rect rc;
		XWindowAttributes xwa;
		XGetWindowAttributes(WindowManger::GetInstance()->display_, handle_, &xwa);
		rc.left = xwa.x;
		rc.top = xwa.y;
		rc.right = rc.left + xwa.width;
		rc.bottom = rc.top + xwa.height;
#else
		Rect rc;
		rc.left = rc.top = 0;
		rc.right = width_;
		rc.bottom = height_;
#endif
		return rc;
	}
	


	WindowManger* WindowManger::global_window_manger_ = NULL;
	WindowManger::WindowManger()
	{
		if (global_window_manger_)
			throw ApplicationException("Engine exception","Window manger instance has been exist");
		global_window_manger_ = this;
#ifdef TNG_USE_X11
		display_ = XOpenDisplay(NULL);
		display_width_ = DisplayWidth(
			display_,
			DefaultScreen( display_ )
			);
		display_heigh_ = DisplayHeight(
			display_,
			DefaultScreen( display_ )
			);
#ifdef HASXGL
		GLint majorGLX, minorGLX = 0;
		glXQueryVersion(display_, &majorGLX, &minorGLX);
		if (majorGLX == 2 && minorGLX < 0)
		{
			throw tng::RuntimeException("GLX 2.0 or greater is required"); 
		}

		int configAttribs[] = 
		{
			GLX_RENDER_TYPE,    GLX_RGBA_BIT,
			GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
			GLX_X_RENDERABLE,   True,
			GLX_DEPTH_SIZE,     24,
			GLX_STENCIL_SIZE,   8,
			GLX_RED_SIZE,       8,
			GLX_GREEN_SIZE,     8,
			GLX_BLUE_SIZE,      8,
			GLX_DOUBLEBUFFER,   True,
			GLX_SAMPLE_BUFFERS, 0,
			GLX_SAMPLES,        0,
			0
		};

		GLXFBConfig* configs;
		int configCount = 0;
		configs = glXChooseFBConfig(display_, DefaultScreen(display_), configAttribs, &configCount);
		if ( configCount == 0 || configs == 0 )
		{
			throw tng::RuntimeException( "glXChooseFBConfig" );
		}

		visual_ = glXGetVisualFromFBConfig(display_, configs[0]);

#endif
#endif // TNG_USE_X11

	}
	WindowManger::~WindowManger()
	{
		global_window_manger_ = NULL;
#ifdef TNG_USE_X11
		XCloseDisplay(display_);
#endif
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
}
#endif