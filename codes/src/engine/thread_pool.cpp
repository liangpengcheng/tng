#include "engine/thread_pool.h"
#include "core/os.h"
namespace tng
{
	ThreadPool::~ThreadPool()
	{
		Quit();
		for (s32 i=0;i<threads_.size();i++)
		{
			threads_[i]->Join();
			delete threads_[i];
		}
	}
	void ThreadPool::Initialize(s32 num)
	{
		for (s32 i =0;i< num;i++)
		{
			Thread* thread=  new Thread(ThreadPool::asyncTask,this);
			threads_.push_back(thread);
		}
	}

	void ThreadPool::AddTask(Task* t)
	{
		task_lock_.lock();
		task_.push_back(t);
		task_lock_.unlock();
		task_cond_.notify_once();
	}

	void ThreadPool::asyncTask(void* param)
	{
		ThreadPool* pool = (ThreadPool*)(param);
		if (pool)
		{
			while (pool->active_)
			{
				pool->task_lock_.lock();
				s32 sz = (s32)pool->task_.size();
				if (sz==0)
				{
					pool->task_cond_.wait(pool->task_lock_);
				}
				if (pool->task_.size()==0)
				{
					pool->task_lock_.unlock();
					continue;
				}
				Task* task = (*pool->task_.begin());
				pool->task_.erase(pool->task_.begin());
				pool->task_lock_.unlock();

				task->Request();
				pool->done_lock_.lock();
				pool->done_.push_back(task);
				pool->done_lock_.unlock();
			}
		}
	}

	void ThreadPool::UpdateDoneList()
	{
		done_lock_.lock();
		for (std::list<Task*>::iterator it = done_.begin();
			it!=done_.end();
			it++)
		{
			(*it)->Response();
			delete *it;
		}
		done_.clear();
		done_lock_.unlock();
	}

	void ThreadPool::Create(int thread_count)
	{
		if (thread_count <= 0)
			thread_count = std::thread::hardware_concurrency();
		thread_count_ = thread_count;
		threads_.resize(thread_count_);
		for (int i = 0; i < thread_count_; i++)
		{
			threads_[i] = std::thread(&ThreadPool::Run, this);
		}
	}

	void ThreadPool::Destroy()
	{
		for (int i = 0; i < thread_count_; i++)
		{
			if (threads_[i].joinable())
				threads_[i].join();
		}
		threads_.clear();
	}
}