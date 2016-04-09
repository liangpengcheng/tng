
#ifdef _WIN32
#include "glesrender/gles_render.h"
#include "core/log.h"

//#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"OpenGL32.lib")

namespace tng
{
	static HGLRC __hrc = 0;
	void GLESRender::OnDestroy(){
		wglDeleteContext(__hrc);
		__hrc = NULL;
	}
	void GLESRender::OnDestroySurface()
	{
		
	}
	void GLESRender::CreateRenderDevice(TngWindow* mainwnd)
	{
		Assert(RenderConfig::GetInstance().back_buffer_sizex_);
		Assert(mainwnd->GetHandle());
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = RenderConfig::GetInstance().colorsize_;
		pfd.cDepthBits = RenderConfig::GetInstance().depthsize_;
		pfd.cStencilBits = RenderConfig::GetInstance().stencilsize_;
		pfd.iLayerType = PFD_MAIN_PLANE;
		HDC hdc= GetDC(mainwnd->GetHandle());
		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0)
		{
			throw tng::RuntimeException("unknown pixel format");
		}

		if (!SetPixelFormat(hdc, pixelFormat, &pfd))
		{
			throw tng::RuntimeException("Failed to set the pixel format");
		}

		HGLRC tempContext = wglCreateContext(hdc);
		if (!tempContext)
		{
			throw tng::RuntimeException("Failed to create temporary context for initialization.");
		}
		wglMakeCurrent(hdc, tempContext);

		// Initialize GLEW
		if (GLEW_OK != glewInit())
		{
			wglDeleteContext(tempContext);
			throw tng::RuntimeException("Failed to initialize GLEW.");

		}

		// Choose pixel format using wglChoosePixelFormatARB, which allows us to specify

		//
		// Note: Keep multisampling attributes at the start of the attribute lists since code below

		int attribList[] = {
			WGL_SAMPLES_ARB, RenderConfig::GetInstance().multisampling_ ? RenderConfig::GetInstance().multisampling_ : 0,
			WGL_SAMPLE_BUFFERS_ARB,  RenderConfig::GetInstance().multisampling_ > 0 ? 1 : 0,
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, RenderConfig::GetInstance().colorsize_,
			WGL_DEPTH_BITS_ARB, RenderConfig::GetInstance().depthsize_,
			WGL_STENCIL_BITS_ARB, RenderConfig::GetInstance().stencilsize_,
			0
		};
		

		UINT numFormats;
		if ( !wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats) || numFormats == 0)
		{
			bool valid = false;
			if (RenderConfig::GetInstance().multisampling_ > 0)
			{
				while (RenderConfig::GetInstance().multisampling_ > 0)
				{
					RenderConfig::GetInstance().multisampling_ /= 2;
					attribList[1] = RenderConfig::GetInstance().multisampling_;
					attribList[3] = RenderConfig::GetInstance().multisampling_ > 0 ? 1 : 0;
					if (wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats) && numFormats > 0)
					{
						valid = true;

						break;
					}
				}
			}

			if (!valid)
			{
				wglDeleteContext(tempContext);

				throw tng::RuntimeException("Failed to choose a pixel format.");
			}
		}

		// Set final pixel format for window
		if (!SetPixelFormat(hdc, pixelFormat, &pfd))
		{
			throw tng::RuntimeException("Failed to set the pixel format");
		}

		// Create our new GL context
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			0
		};

		if (!(__hrc = wglCreateContextAttribsARB(hdc, 0, attribs) ) )
		{
			wglDeleteContext(tempContext);
			throw tng::RuntimeException("Failed to create OpenGL context.");
		}

		// Delete the old/temporary context and window
		wglDeleteContext(tempContext);

		// Make the new context current
		if (!wglMakeCurrent(hdc, __hrc) || !__hrc)
		{
			throw tng::RuntimeException("Failed to make the window current.");
		}
		/**
		The function pointers you get are based on the current context. That is, it is entirely possible for you to get different function pointers for different OpenGL contexts. gDEBugger is telling you that it has detected that you're using function pointers with a different context from the one used to acquire them. Which is not guaranteed to work.

		That being said, it generally will work. It won't work if the two contexts aren't from the same vendor (and probably for the same GPU or SLI/CrossFire GPU setup). But if they are, it should be fine.

		GLEW, as I understand it, doesn't have a way to accommodate two different contexts that don't use the same function pointers. You would have to call glewInit every time you changed contexts.
		*/
		// Initialize GLEW
		if (GLEW_OK != glewInit())
		{
			wglDeleteContext(__hrc);
			throw tng::RuntimeException("Failed to initialize GLEW.");

		}

		// Vertical sync.
		wglSwapIntervalEXT(RenderConfig::GetInstance().vsync_ ? 1 : 0);
		Log::GetLog()->Printf(Log::SYS_CHN,"opengl es render created");
		SetDepthRange(0,1);
		_loadExtentions();

	}
	void GLESRender::Swap(TngWindow* wnd)
	{
		Assert(wnd);
		HDC dc = ::GetDC(wnd->GetHandle());
		if(dc)
		{
			SwapBuffers(dc);
			::ReleaseDC(wnd->GetHandle(),dc);
		}
		
	}
}
#endif