#include "core/arch_detect.h"
#include "core/log.h"
#if TNG_OS_FAMILY_UNIX
#include "graphics/graphics.h"
namespace tng{

#ifdef TNG_USE_X11

	void TngWindow::UpdateEvent()
	{
		while (XEventsQueued(WindowManger::GetInstance()->display_, QueuedAlready))
		{
			XEvent event;
			XNextEvent(WindowManger::GetInstance()->display_, &event);
		}

	}

#endif

	WINHANDLE TngWindow::Create(const string& appname, u32 width, u32 height,u32 style)
	{
#ifdef TNG_USE_X11
		Assert(WindowManger::GetInstance()->display_);

#ifdef HASXGL
		XSetWindowAttributes winAttribs;
		long eventMask;
		eventMask = ExposureMask | VisibilityChangeMask | StructureNotifyMask |
			KeyPressMask | KeyReleaseMask | PointerMotionMask |
			ButtonPressMask | ButtonReleaseMask |
			EnterWindowMask | LeaveWindowMask;
		winAttribs.event_mask = eventMask;
		winAttribs.border_pixel = 0;
		winAttribs.bit_gravity = StaticGravity;

		winAttribs.colormap = XCreateColormap(WindowManger::GetInstance()->display_, RootWindow(WindowManger::GetInstance()->display_, WindowManger::GetInstance()->visual_->screen), WindowManger::GetInstance()->visual_->visual, AllocNone);
		GLint winMask;
		winMask = CWBorderPixel | CWBitGravity | CWEventMask | CWColormap;

		handle_ = XCreateWindow(WindowManger::GetInstance()->display_, DefaultRootWindow(WindowManger::GetInstance()->display_), 0, 0, width, height, 0,
			WindowManger::GetInstance()->visual_->depth, InputOutput, WindowManger::GetInstance()->visual_->visual, winMask,
			&winAttribs);


#else
		handle_ = XCreateWindow((Display*)WindowManger::GetInstance()->display_,
			DefaultRootWindow((Display*)WindowManger::GetInstance()->display_),
			0, 0, width, height, 0, CopyFromParent,
			CopyFromParent, CopyFromParent, 0, 0);




#endif

		XSizeHints theSizeHints;

		theSizeHints.flags = PPosition | PSize;
		theSizeHints.x = 0;
		theSizeHints.y = 0;
		theSizeHints.width = width;
		theSizeHints.height = height;


		XSetNormalHints(WindowManger::GetInstance()->display_, handle_, &theSizeHints);

		XMapWindow(WindowManger::GetInstance()->display_, handle_);

		XStoreName(WindowManger::GetInstance()->display_, handle_, appname.c_str());

		XFlush(WindowManger::GetInstance()->display_);

#endif
		name_ = appname;
		GetWinMap()[handle_] = this;
		return handle_;
	}

}
#endif // 
