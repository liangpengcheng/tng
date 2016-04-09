#include "graphics/graphics.h"
#include "engine/engine.h"
#include "core/os.h"
#include "graphics/renderservice.h"
namespace tng
{

	GFXService*		GFXService::global_gfx_ = NULL;
	GFXService::GFXService():render_(NULL){
		
	}
	GFXService::~GFXService(){
#ifndef NO_RENDER_THREAD
		Engine::GetInstance()->GetLoop(RSLOOP)->WaitThreadEnd();
#endif
		delete scene_manger_;
		scene_manger_ = NULL;
		delete mtl_mgr_;
		if (render_)
		{
			delete render_;
		}
		global_gfx_ = NULL;
#ifdef ANDROID
		SAFE_DELETE(main_window);
#endif // ANDROID
	}
	void GFXService::Exit()
	{
		Engine::GetInstance()->OnExisting();
		ResourceManger::DestroyManger();
#ifndef NO_RENDER_THREAD
		Engine::GetInstance()->GetLoop(RSLOOP)->DestoryNextFrame();
#endif
		Engine::GetInstance()->GetLoop(WINLOOP)->DestoryNextFrame();
		Engine::GetInstance()->GetDefaultLoop()->DestoryNextFrame();
#ifndef NO_RENDER_THREAD
		internal::GetRenderService()->NotifyMe();
#endif
	}
	void GFXService::OnCreate()
	{
		// resource manger initialize
		res_mgr_ = ResourceManger::CreateManger();
		res_mgr_->SetLoadingThread(1);
		Log::GetLog()->Printf(Log::SYS_CHN, "resource loading thread num: 1");
		ResourceManger::GetInstance()->AddLoader(LocalFilesLoader);
		//ResourceManger::GetInstance()->SetAssetPath("../SampleAsset/");


		if (global_gfx_)
			throw ApplicationException("GFXService exception", "GFXService instance has been exist");
		global_gfx_ = this;
		scene_manger_ = new SceneManger;

#ifndef NO_RENDER_THREAD
#ifndef ANDROID
		Loop* winloop = Engine::GetInstance()->CreateLoop(WINLOOP);
		window_manger_ = Engine::GetInstance()->CreateService("WindowManger", WINMGR, WINLOOP)->DynamicCast<WindowManger>();
		window_manger_->SetTimer(0.03f);
		winloop->Start();
#endif
		Engine::GetInstance()->CreateLoop(RSLOOP);
		render_service_ = Engine::GetInstance()->CreateService("RenderService", "RenderService", RSLOOP)->DynamicCast<RenderService>();
		render_service_->SetTimer(0.02f);
#else
		window_manger_ = Engine::GetInstance()->CreateService("WindowManger", WINMGR, "DefaultLoop")->DynamicCast<WindowManger>();
		render_service_ = Engine::GetInstance()->CreateService("RenderService", "RenderService", "DefaultLoop")->DynamicCast<RenderService>();
#endif

		mtl_mgr_ = new MaterialManger();
		mtl_mgr_->Initialize();
		mtl_mgr_->LoadPath("engine:/Material");
	}
	bool GFXService::Initialize(Loop* loop)
	{
		bool ret = UpdateableService::Initialize(loop);
		Log::GetLog()->Printf(Log::SYS_CHN, "graphics service initialized");
		return ret;
	}
	void GFXService::SetRender(Render* r)
	{
		render_ = r;
		render_service_->SetRender(r);
#ifndef NO_RENDER_THREAD
		Engine::GetInstance()->GetLoop(RSLOOP)->Start();
#else
		render_->CreateRenderDevice(GFXService::GetInstance()->GetMainWindow());
		render_->InitializeDefaultState();
#endif
	}
	void GFXService::_update(f32 deltaTime)
	{
		scene_manger_->Update(deltaTime);
#ifdef ANDROID
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident = ALooper_pollAll(Engine::GetInstance()->IsRunning()? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(OS::state__, source);
			}
		}
#endif
	}
	GFXService* GFXService::CreateGraphics()
	{
		Engine* pengine = Engine::Create();
		GFXService* service = (GFXService*)pengine->CreateService("GFXService", "gfx_service", "DefaultLoop");
		service->OnCreate();
		service->SetTimer(0.02f);
		Log::GetLog()->Printf(Log::SYS_CHN, "graphics service created");
		return service;
	}
	void GFXService::CreateMainRenderWindow(const string& appname,u32 width, u32 height,u32 style)
	{
#ifdef ANDROID
		//先确定窗口初始化了
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while (!OS::state__->window)
		{
			int ident;
			int events;
			struct android_poll_source* source;
			while ((ident = ALooper_pollAll(Engine::GetInstance()->IsRunning()? 0 : -1, NULL, &events,
				(void**)&source)) >= 0) {

				// Process this event.
				if (source != NULL) {
					source->process(OS::state__, source);
				}
			}
		}
		Log::GetLog()->Printf(Log::SYS_CHN, "Main Activity Initilized");
		main_window= new TngWindow();
#else
#ifndef NO_RENDER_THREAD
		GetWindowManger()->CreateWnd(appname, width, height, style);
		while(!(main_window =window_manger_->FindWindow(appname)))
		{
			OS::Sleep(0.01f);
		}
#else
		main_window=GetWindowManger()->_createwnd_internel(appname, width, height, style);
#endif
		
#endif
		main_window->SetMain(true);
	}
	START_SUB_CLASS(GFXService, UpdateableService)
	END_CLASS


#ifdef TNG_ANDROID

#endif
}