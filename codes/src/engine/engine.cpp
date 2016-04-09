#include "engine/engine.h"
#include "core/log.h"
#include "core/path.h"
#include "uv/include/uv.h"
namespace tng
{
	AutoPtr<Engine>	Engine::global_engine_;
	Engine::Engine() :app_name_("org.borg_tng.default"){}
	Engine::~Engine(){}
	Engine* Engine::Create()
	{
		if (!global_engine_.isNull())
			throw ApplicationException("Engine exception","Engine instance has been exist");
		string wp =Path::GetWorkPath();
		Log::GetLog()->Printf(Log::SYS_CHN, "Engine starting...[working path:%s]", wp.c_str());
		global_engine_ = new Engine;
		DefaultLoop* pLoop = new DefaultLoop;
		pLoop->Create();
		global_engine_->InsertLoop("DefaultLoop",pLoop);

		Log::GetLog()->Printf(Log::SYS_CHN, "Engine Created");
		global_engine_->current_state_ = Running; 
		return GetInstance();
	}
	void Engine::Destory()
	{
		for (LoopMap::iterator it = GetInstance()->loop_map_.begin();
			it!=GetInstance()->loop_map_.end();
			it++)
		{
			delete it->second;
		}
		GetInstance()->loop_map_.clear();
		global_engine_ = NULL;
	}

	void Engine::InsertLoop(const string& loopname,Loop* loop)
	{
		if (loop_map_.find(loopname)==loop_map_.end())
			loop_map_[loopname] = loop;
		else
		{
			string err= string("loop has been exist:")+loopname;
			throw ApplicationException("Engine exception",err);
		}
	}

	Loop* Engine::CreateLoop(const string& loopname)
	{
		Loop* retLoop = new Loop;
		retLoop->Create();
		InsertLoop(loopname,retLoop);
		//Log::GetLog()->Printf(Log::SYS_CHN,"loop [%s] created",loopname.c_str());
		return retLoop;
	}
	Loop* Engine::GetLoop(const string& loopname)
	{
		LoopMap::iterator loopit = loop_map_.find(loopname);
		if (loopit!=loop_map_.end())
			return loopit->second;
		else
			return NULL;
	}
	void Engine::DestoryLoop(const string& loopname)
	{
		Loop* loop = GetLoop(loopname);
		if (loop)
		{
			delete loop;
			loop_map_.erase(loopname);
			//Log::GetLog()->Printf(Log::SYS_CHN, "loop [%s] destoried", loopname.c_str());
		}
	}
	Service* Engine::CreateService(const string& classname,const string& servicename,const string& belongloop)
	{
		Loop* pLoop = GetLoop(belongloop);
		if (pLoop)
		{
			Object* pObj = Object::CreateObject(classname);
			Log::GetLog()->Printf(Log::SYS_CHN, "service [class:%s][name:%s] created", classname.c_str(), servicename.c_str());
			if (pObj)
			{
#ifdef USE_RTTI
				Service* pSer = pObj->DynamicCast<Service>();
#else
				Service* pSer = (Service*)pObj;
#endif // USE_RTTI

				
				if (pSer)
				{
					pLoop->AddService(servicename, pSer);
					return pSer;
				}
				else
				{
					delete pObj;
					throw BadCastException("can't convert to Service object",classname);
				}
			}
		}
		return NULL;
	}
	Service*	Engine::GetService(const string& servicename, const string& belongloop)
	{
		Loop* loop = GetLoop(belongloop);
		if (loop)
			return loop->GetService(servicename);
		else
			return NULL;
	}
	void	Engine::DestoryService(const string& servicename, const string& belongloop)
	{
		Loop* loop = GetLoop(belongloop);
		if (loop)
			loop->RemoveService(servicename);
		//Log::GetLog()->Printf(Log::SYS_CHN, "service [%s] removed", servicename.c_str());
	}
	Loop* Engine::GetDefaultLoop()
	{
		return GetLoop("DefaultLoop");
	}
	void Engine::MainLoop()
	{
		GetDefaultLoop()->Start();
	}
// 	static void work_cb(uv_work_t* req) {
// 		Task* task = (Task*)req->data;
// 		task->Request();
// 	}
// 
// 
// 	static void after_work_cb(uv_work_t* req, int status) {
// 		Assert(status == 0);
// 		Task* task = (Task*)req->data;
// 		task->Response();
// 		delete req;
// 	}
// 	void DoTask(Task* task,const string& loop_name)
// 	{
// 		Loop* loop = Engine::GetInstance()->GetLoop(loop_name);
// 		uv_work_t* work = new uv_work_t;
// 		work->data = task;
// 		uv_queue_work(loop->_getInternalLoop(), work, work_cb, after_work_cb);
// 		//task->Request();
// 		//task->Response();
// 	}
}