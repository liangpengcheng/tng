#ifdef ANDROID
#include "glesrender/gles_render.h"
#include "engine/engine.h"
#include "core/log.h"
#include "glesrender/gles_render_macros.h"
#include "graphics/graphics.h"
#include "graphics/rendercontex.h"
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays = NULL;
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays = NULL;
PFNGLISVERTEXARRAYOESPROC glIsVertexArray = NULL;
namespace tng
{
	GLESRender* render__ = NULL;
	static TngWindow* mainwnd__ = NULL;
	// Process the next main command.
	static void engine_handle_cmd(struct android_app* app, int32_t cmd)
	{
		Assert(mainwnd__ );
		mainwnd__ ->engine_handle_cmd(app,cmd);
	}

	// Process the next input event.
		static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
	{
			Assert(mainwnd__ );
			return mainwnd__ ->engine_handle_input(app,event);

	}
	static EGLenum checkErrorEGL(const char* msg)
	{
		static const char* errmsg[] =
		{
			"EGL function succeeded",
			"EGL is not initialized, or could not be initialized, for the specified display",
			"EGL cannot access a requested resource",
			"EGL failed to allocate resources for the requested operation",
			"EGL fail to access an unrecognized attribute or attribute value was passed in an attribute list",
			"EGLConfig argument does not name a valid EGLConfig",
			"EGLContext argument does not name a valid EGLContext",
			"EGL current surface of the calling thread is no longer valid",
			"EGLDisplay argument does not name a valid EGLDisplay",
			"EGL arguments are inconsistent",
			"EGLNativePixmapType argument does not refer to a valid native pixmap",
			"EGLNativeWindowType argument does not refer to a valid native window",
			"EGL one or more argument values are invalid",
			"EGLSurface argument does not name a valid surface configured for rendering",
			"EGL power management event has occurred",
		};
		EGLenum error = eglGetError();
		Log::GetLog()->Printf(Log::ERROR_CHN,"%s: %s.", msg, errmsg[error - EGL_SUCCESS]);
		return error;
	}
	static bool __multiSampling = false;
	static EGLDisplay __eglDisplay = EGL_NO_DISPLAY;
	static EGLSurface __eglSurface = EGL_NO_SURFACE;
	static EGLContext __eglContext = EGL_NO_CONTEXT;
	static EGLConfig __eglConfig = 0;
	
	static void destroyEGLSurface()
	{
		if (__eglDisplay != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(__eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		}

		if (__eglSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(__eglDisplay, __eglSurface);
			__eglSurface = EGL_NO_SURFACE;
		}
		
	}
	void GLESRender::OnDestroySurface()
	{
		destroyEGLSurface();
	}
	void GLESRender::CreateRenderDevice(TngWindow* mainwnd)
	{
		render__ = this;
		mainwnd__ = mainwnd;
		Log::GetLog()->Printf(Log::DBG_CHN,"pre create gles");
		int samples = RenderConfig::GetInstance().multisampling_ ? RenderConfig::GetInstance().multisampling_ : 0;
		
		EGLint eglConfigAttrs[] =
		{
			EGL_SAMPLE_BUFFERS,     samples > 0 ? 1 : 0,
			EGL_SAMPLES,            samples,
			EGL_DEPTH_SIZE,         24,
			EGL_RED_SIZE,           8,
			EGL_GREEN_SIZE,         8,
			EGL_BLUE_SIZE,          8,
			EGL_ALPHA_SIZE,         8,
			EGL_STENCIL_SIZE,       8,
			EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};

		__multiSampling = samples > 0;

		EGLint eglConfigCount;
		const EGLint eglContextAttrs[] =
		{
			EGL_CONTEXT_CLIENT_VERSION,    2,
			EGL_NONE
		};

		const EGLint eglSurfaceAttrs[] =
		{
			EGL_RENDER_BUFFER,    EGL_BACK_BUFFER,
			EGL_NONE
		};
		if (__eglDisplay == EGL_NO_DISPLAY && __eglContext == EGL_NO_CONTEXT)
		{
			// Get the EGL display and initialize.
 
			__eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (__eglDisplay == EGL_NO_DISPLAY)
			{
				checkErrorEGL("eglGetDisplay");
				return;
			}


			if (eglInitialize(__eglDisplay, NULL, NULL) != EGL_TRUE)
			{
				checkErrorEGL("eglInitialize");
				return;
			}
			else
				Log::GetLog()->Printf(Log::DBG_CHN,"egl Initialized");

			// Try both 24 and 16-bit depth sizes since some hardware (i.e. Tegra) does not support 24-bit depth
			bool validConfig = false;
			EGLint depthSizes[] = { 24, 16 };
			for (unsigned int i = 0; i < 2; ++i)
			{
				eglConfigAttrs[1] = samples > 0 ? 1 : 0;
				eglConfigAttrs[3] = samples;
				eglConfigAttrs[5] = depthSizes[i];
				/* Here, the application chooses the configuration it desires. 
				*/  

				if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
				{
					validConfig = true;
					break;
				}
	
				if (samples)
				{
					// Try lowering the MSAA sample size until we find a supported config
					int sampleCount = samples;
					while (sampleCount)
					{
						Log::GetLog()->Printf(Log::WARNING_CHN,"No EGL config found for depth_size=%d and samples=%d. Trying samples=%d instead.", depthSizes[i], sampleCount, sampleCount / 2);
						sampleCount /= 2;
						eglConfigAttrs[1] = sampleCount > 0 ? 1 : 0;
						eglConfigAttrs[3] = sampleCount;
						if (eglChooseConfig(__eglDisplay, eglConfigAttrs, &__eglConfig, 1, &eglConfigCount) == EGL_TRUE && eglConfigCount > 0)
						{
							validConfig = true;
							break;
						}
					}

					__multiSampling = sampleCount > 0;

					if (validConfig)
						break;
				}
				else
				{
					Log::GetLog()->Printf(Log::WARNING_CHN,"No EGL config found for depth_size=%d.", depthSizes[i]);
				}
			}

			if (!validConfig)
			{
				checkErrorEGL("eglChooseConfig");
				return;
			}else
				Log::GetLog()->Printf(Log::DBG_CHN,"eglChooseConfig");

			__eglContext = eglCreateContext(__eglDisplay, __eglConfig, EGL_NO_CONTEXT, eglContextAttrs);
			if (__eglContext == EGL_NO_CONTEXT)
			{
				checkErrorEGL("eglCreateContext");
				return;
			}
			else
				Log::GetLog()->Printf(Log::DBG_CHN,"eglCreateContext");
		}
		// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		// guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		// As soon as we picked a EGLConfig, we can safely reconfigure the
		// ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
		EGLint format;
		eglGetConfigAttrib(__eglDisplay, __eglConfig, EGL_NATIVE_VISUAL_ID, &format);
		Log::GetLog()->Printf(Log::DBG_CHN,"state addr=%d.",OS::state__);
		Log::GetLog()->Printf(Log::DBG_CHN,"wnd addr=%d.",OS::state__->window);
		ANativeWindow_setBuffersGeometry(OS::state__->window, 0, 0, format);
		__eglSurface = eglCreateWindowSurface(__eglDisplay, __eglConfig, OS::state__->window, eglSurfaceAttrs);
		if (__eglSurface == EGL_NO_SURFACE)
		{
			checkErrorEGL("eglCreateWindowSurface");
			return;
		}
		else
			Log::GetLog()->Printf(Log::DBG_CHN,"eglCreateWindowSurface");

		if (eglMakeCurrent(__eglDisplay, __eglSurface, __eglSurface, __eglContext) != EGL_TRUE)
		{
			checkErrorEGL("eglMakeCurrent");
			return;
		}
		else
			Log::GetLog()->Printf(Log::DBG_CHN,"eglMakeCurrent");

		EGLint szx = RenderConfig::GetInstance().back_buffer_sizex_;
		EGLint szy = RenderConfig::GetInstance().back_buffer_sizey_;
		eglQuerySurface(__eglDisplay, __eglSurface, EGL_WIDTH, &szx);
		eglQuerySurface(__eglDisplay, __eglSurface, EGL_HEIGHT, &szy);
		Log::GetLog()->Printf(Log::SYS_CHN, "target device width(%d),heigh(%d)",szx,szy);
		mainwnd->width_ = szx;
		mainwnd->height_ = szy;

		// Set vsync.
		eglSwapInterval(__eglDisplay, 1);

		// Initialize OpenGL ES extensions.
		_loadExtentions();

		if (_checkExtention("GL_OES_vertex_array_object") || _checkExtention("GL_ARB_vertex_array_object"))
		{
			// Disable VAO extension for now.
			glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
			glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArrays");
			glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
			glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress("glIsVertexArrayOES");
			if (glBindVertexArray)
			{
				Log::GetLog()->Printf(Log::SYS_CHN,"using vao");
			}
			
		}
		Log::GetLog()->Printf(Log::SYS_CHN, "gles device created");
		OS::state__->onAppCmd =engine_handle_cmd;
		OS::state__->onInputEvent =engine_handle_input;
	}

	void GLESRender::Swap(TngWindow* wnd)
	{
		if (__eglDisplay && __eglSurface)
		{
			int rc =eglSwapBuffers(__eglDisplay, __eglSurface);

			if (rc != EGL_TRUE)
			{
				EGLint error = eglGetError();
				if (error == EGL_BAD_NATIVE_WINDOW)
				{
					if (OS::state__->window != NULL)
					{
						destroyEGLSurface();
						Log::GetLog()->Printf(Log::SYS_CHN,"recreate gles surface");
						CreateRenderDevice(wnd);
					}
					else
					{
						Log::GetLog()->Printf(Log::SYS_CHN,"widow is null");
					}
				}
				else
				{
					Log::GetLog()->Printf(Log::SYS_CHN,"other error %d",error);
				}
			}
// 			else
// 			{
// 				Log::GetLog()->Printf(Log::DBG_CHN,"swap");
// 			}
		}

	}
	void GLESRender::OnDestroy()
	{
		if ( __eglDisplay != EGL_NO_DISPLAY )  
		{  

			eglMakeCurrent(__eglDisplay,
				EGL_NO_SURFACE,
				EGL_NO_SURFACE,
				EGL_NO_CONTEXT);

			if (__eglContext != EGL_NO_CONTEXT)
			{

				eglDestroyContext(__eglDisplay,
					__eglContext);
			}

			if (__eglSurface != EGL_NO_SURFACE)
			{
				eglDestroySurface(__eglDisplay,
					__eglSurface);
			}

			eglTerminate(__eglDisplay);
		}

		__eglDisplay = EGL_NO_DISPLAY;
		__eglContext = EGL_NO_CONTEXT;
		__eglSurface = EGL_NO_SURFACE;
	}
}
#endif