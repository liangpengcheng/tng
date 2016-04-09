#include "engine/threadloop.h"
#include "engine/engine.h"
#include "core/core_macros.h"
#include "core/runtime_check.h"
#include "engine/service.h"
#include "core/math.h"
#include "core/os.h"


namespace tng {
	Loop::Loop():
		destory_(false),
		work_thread_(NULL)
	{
		lastUpdateTime_ = Clock::Tick();
	}
	Loop::~Loop()
	{
		Destory();
	}
  
	bool Loop::Create()
	{
		return true;
	}
	void Loop::WaitThreadEnd()
	{
		if (work_thread_)
		{
			work_thread_->Join();
		}
	}
	void Loop::Destory()
	{
		service_lock_.lock();
		destory_ = true;

		WaitThreadEnd();
		for (ServiceMap::iterator it = service_map_.begin(); it != service_map_.end(); it++)
		{
			delete it->second;
		}
		service_map_.clear();
		service_lock_.unlock();
		SAFE_DELETE(work_thread_);
	}
	void Loop::Start()
	{
		createThread();
	}
	void Loop::Update(void*)
	{
		service_lock_.lock();
		while(!destory_&&service_map_.size())
		{
			service_lock_.unlock();
			service_lock_.lock();
			for (int i = 0 ;i < deletelist_.size() ;i ++)
			{
				ServiceMap::iterator it = service_map_.find(deletelist_[i]);
				if (it != service_map_.end())
				{
					it->second->OnDestory();
					delete it->second;
					service_map_.erase(it);
				}
			}
			deletelist_.clear();
			f64 sleeptitme = 600.0f;
			tick_t prethistime = Clock::Tick();
			for (ServiceMap::iterator it= service_map_.begin();it!=service_map_.end();it++)
			{
				Service *serv = it->second;
				if(serv)
				{
					if (!serv->Initialized())
					{
						serv->Initialize(this);
					}
#ifdef USE_RTTI
					UpdateableService* updateserv = serv->DynamicCast<UpdateableService>();
#else
					UpdateableService* updateserv = (UpdateableService*)serv;
#endif
					
					if (updateserv)
					{
						
						f32 deltaTime = Clock::TimespanToSecondsF(updateserv->lastUpdateTime_,prethistime);
						
						sleeptitme = Min<f64>(sleeptitme,updateserv->update_time_);
						updateserv->lastUpdateTime_ = prethistime;
						updateserv->_update(deltaTime);
						
					}
				}
			}
			sleeptitme = sleeptitme - Clock::SecondsSinceF(prethistime);
			if(sleeptitme>.0f)
				OS::Sleep((f32)sleeptitme);
			lastUpdateTime_ = prethistime;
		}
		//thread exiting call Sevice::OnDestroy
		for (ServiceMap::iterator it= service_map_.begin();it!=service_map_.end();it++)
		{
			Service *serv = it->second;
			if(serv)
			{
#ifdef USE_RTTI
				UpdateableService* updateserv = serv->DynamicCast<UpdateableService>();
				if (updateserv)
#else
				UpdateableService* updateserv = (UpdateableService*)serv;
#endif
				{
					updateserv->OnDestory();
				}
				delete updateserv;
			}
		}
		service_map_.clear();
		service_lock_.unlock();
	}
	void Loop::createThread()
	{
		Thread::ThreadFun fun1(this,&Loop::Update);
		work_thread_ = new tng::Thread(fun1,NULL);
	}
	Service* Loop::GetService(const string& service)
	{
		AutoLock<Mutex> auto_lock(&service_lock_);
		ServiceMap::iterator it = service_map_.find(service);
		if (it != service_map_.end())
			return it->second;
		else
			return NULL;
	}
	void Loop::RemoveService(const string& service)
	{
		AutoLock<Mutex> auto_lock(&service_lock_);
		deletelist_.push_back(service);
	}

	void Loop::AddService(const string& service_name, Service* service)
	{
		AutoLock<Mutex> auto_lock(&service_lock_);
		Assert(service_map_.find(service_name) == service_map_.end());
		service_map_[service_name] = service;
		
	}

	void DefaultLoop::Start()
	{
		Update(NULL);
	}

//	bool DefaultLoop::Create()
//	{
//#ifndef TNG_NO_UVLOOP
//		if (internal_loop_)
//			throw ApplicationException("Engine exception","loop has been exist");
//		internal_loop_ = uv_default_loop();
//		return internal_loop_?true:false;
//#else
//		return true;
//#endif
//	}
}