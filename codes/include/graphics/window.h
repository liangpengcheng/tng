#ifndef _tng_window_h__
#define _tng_window_h__

#include "graphics/graphics_macros.h"
#include "engine/event_dispatcher.h"

#include "engine/service.h"
#include <map>
#ifdef TNG_USE_X11
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#endif
#ifdef TNG_ANDROID
#include <EGL/eglplatform.h>
#include "core/android_native_app_glue.h"
#include "android/native_window.h"
#endif
#include "core/math.h"
#include "MathGeoLib/Math/Rect.h"
#include "graphics/input.h"
namespace tng
{
#ifdef TNG_OS_FAMILY_WINDOWS
	typedef HWND				WINHANDLE;
#define FULL_SCREEN_STYLE			WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE
#define NORMAL_STYLE				WS_OVERLAPPEDWINDOW | WS_VISIBLE

#elif defined(TNG_USE_X11)
	#define FULL_SCREEN_STYLE		0
	#define NORMAL_STYLE			0
	typedef Window 				WINHANDLE;
#elif defined(TNG_ANDROID)
	typedef EGLNativeWindowType WINHANDLE;
	#define FULL_SCREEN_STYLE		0
	#define NORMAL_STYLE			0
#else
	#define FULL_SCREEN_STYLE		0
	#define NORMAL_STYLE			0
	typedef void* 				WINHANDLE;
#endif
#define WINLOOP					"WindowtLoop"
#define WINMGR					"WindowManger"
	class GFX_API TngWindow :public EventDispatcher
	{
	public:
		friend class WindowManger;
		TngWindow();
		~TngWindow();
		WINHANDLE	Create(const string& appname,u32 width, u32 height,u32 style);

		typedef unordered_map<WINHANDLE, TngWindow*>	WinMap;
		WINHANDLE	GetHandle(){return handle_;}
		Rect		GetRect();
		void		SetMain(bool m){main_ = m;}
		bool		IsMain()const{return main_;}
	protected:
		///if main window is destroyed the application will exit next frame
		bool									main_;
		WINHANDLE								handle_;
		string								name_;
		static WinMap& GetWinMap(){
		  static WinMap winmap_;
		  return winmap_;
		}
#ifdef TNG_OS_FAMILY_WINDOWS
		static LRESULT CALLBACK				DefultCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#ifdef TNG_USE_X11
		static void UpdateEvent();
#endif
#ifdef TNG_ANDROID
	public:

		u32 width_ ;
		u32 height_;

		void engine_handle_cmd(struct android_app* app, int32_t cmd);
		int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
#endif // ANDROID
	};
	class GFX_API CreateWndCommand:public Command
	{
	public:
		CreateWndCommand():width(0),height(0),style(0){}
		virtual bool Do();
		string appname;
		u32 width;
		u32 height;
		u32 style;
	};

	class GFX_API WindowManger:public UpdateableService
	{
	public:
		ENGINE_CLASS_DEF
		WindowManger();
		~WindowManger();
		typedef unordered_map<string, TngWindow*>	WinMap;
		TngWindow*	_createwnd_internel(const string& appname,u32 width, u32 height,u32 style);
		void	CreateWnd(const string& appname,u32 width, u32 height,u32 style);
		//TngWindow* CreateWndIME(const string& appname,u32 width, u32 height,u32 style);
		/// close window
		void	ShutWindow(const string& appname);
		///the window removed will not process message ,but still exit
		void	RemoveWindow(const string& appname);

		TngWindow* FindWindow(const string& appname);

		static WindowManger*				GetInstance(){return global_window_manger_;}
	protected:
		list<Command*>					commands_;
		virtual void						_update(f32 deltaTime);
		WinMap								winmap_;
		static WindowManger*				global_window_manger_;
		Mutex								winmutex_;
		u32									display_width_;
		u32									display_heigh_;
#ifdef TNG_USE_X11
	public:
		Display*							display_;
#ifdef HASXGL
		XVisualInfo*						visual_;
#endif
#endif // TNG_USE_X11

	};
}

#endif