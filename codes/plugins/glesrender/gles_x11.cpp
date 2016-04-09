#if defined(TNG_USE_X11)
#include "glesrender/gles_render.h"
#include "GL/glx.h"
namespace tng
{
	void GLESRender::OnDestroy(){}
	void GLESRender::CreateRenderDevice(TngWindow* mainwnd)
	{
	  
		// GLX version
		 GLXContext __context = glXCreateContext(WindowManger::GetInstance()->display_, WindowManger::GetInstance()->visual_, NULL, True);
		 GL_ASSERT();
		 if (!__context)
		 {
			 throw tng::RuntimeException("glXCreateContext");
		 }
		 GL_ASSERT(glXMakeCurrent(WindowManger::GetInstance()->display_, mainwnd->GetHandle(), __context));
		 Log::GetLog()->Printf(Log::SYS_CHN,"opengl es render created");
	  glewInit();
		_loadExtentions();
	}

	void GLESRender::Swap(TngWindow* wnd)
	{
		glXSwapBuffers(WindowManger::GetInstance()->display_, wnd->GetHandle());
	}

	void GLESRender::OnDestroySurface()
	{

	}
}
#endif