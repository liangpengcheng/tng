#ifndef _TNG_THREAD_POOL_H_
#define _TNG_THREAD_POOL_H_
#include "engine/engine_macros.h"
#include "core/threadfun.h"
#include <vector>
#include <list>
namespace tng
{
	class ENGINE_API Task
	{
	public:
		virtual ~Task(){}
		virtual void Request()=0;
		virtual void Response() = 0;
	};
	// use thread pool do task
	//ENGINE_FUNC_API void DoTask(Task* task,const std::string& loop_name);

	class ENGINE_API ThreadPool
	{
	public:
		ThreadPool(){
			active_=1;
		}
		~ThreadPool();
		/**
			create thread pool
			@param num thread num
		*/
		void Initialize(s32 num);

		void Quit(){active_ = false;}

		void AddTask(Task* t);

		void UpdateDoneList();
	protected:
		static void asyncTask(void*);
		std::vector<Thread*>		threads_;
		std::list<Task*>			task_;
		std::list<Task*>			done_;
		Mutex						task_lock_;
		Mutex						done_lock_;
#if !defined(NO_LIBUV)&&!defined(EMCC)
		Condition					task_cond_;
#endif
		AtomicCounter				active_;
	};
}

#endif