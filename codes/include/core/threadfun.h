#pragma once
#include "config.h"
#ifndef NO_LIBUV
extern "C"
{
#include "uv/include/uv.h"
};
#endif // !NO_LIBUV


#include "types.h"
#include "returncode.h"
//#include "core/functor.h"
#include "core/delegate.h"
#include "core/memobj.h"
#include "core/runtime_check.h"

#if TNG_OS== TNG_OS_MAC_OS_X
#include <libkern/OSAtomic.h>
#endif

namespace tng
{
	namespace private_
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		typedef HANDLE tng_thread;

		typedef HANDLE tng_sem;

		typedef CRITICAL_SECTION tng_mutex;
#elif defined(EMCC)
		typedef int tng_thread;
#else
#if TNG_OS == TNG_OS_MAC_OS_X
		typedef semaphore_t tng_sem;
#else
		typedef sem_t tng_sem;
#endif
		typedef pthread_t tng_thread;
		typedef pthread_mutex_t tng_mutex;



#endif // TNG_OS == TNG_OS_FAMILY_WINDOWS


	}
	class NullLock :public MemObj
	{
	public:
		__inline void lock() {
		}
		__inline void unlock() {
		}
	};
	class CORE_API Mutex :public MemObj
	{
	public:
		Mutex() {
			uv_mutex_init(&mutex_);
		}
		~Mutex() {
			uv_mutex_destroy(&mutex_);
		}

		void lock() {
			uv_mutex_lock(&mutex_);
		}
		void unlock() {
			uv_mutex_unlock(&mutex_);
		}
		uv_mutex_t  mutex_;
	};
	//#if !defined(NO_LIBUV)&&!defined(EMCC)
	class CORE_API RWLock :public MemObj
	{
	public:
		RWLock() {
			uv_rwlock_init(&rwlock_);
		}
		~RWLock() {
			uv_rwlock_destroy(&rwlock_);
		}
		void rdlock() {
			uv_rwlock_rdlock(&rwlock_);
		}
		void rdunlock() {
			uv_rwlock_rdunlock(&rwlock_);
		}
		void wrlock() {
			uv_rwlock_wrlock(&rwlock_);
		}
		void wrunlock() {
			uv_rwlock_wrunlock(&rwlock_);
		}
	private:
		uv_rwlock_t rwlock_;
	};

	class CORE_API Semaphore :public MemObj
	{
	public:
		Semaphore(u32 val = 0) {
			uv_sem_init(&semaphore_, val);
		}
		~Semaphore() {
			uv_sem_destroy(&semaphore_);
		}

		void wait() {
			uv_sem_wait(&semaphore_);
		}
		s32  trywait() {
			return uv_sem_trywait(&semaphore_);
		}
		void post() {
			uv_sem_post(&semaphore_);
		}

	private:
		uv_sem_t semaphore_;
	};

	class CORE_API Condition :public MemObj
	{
	public:
		Condition()
		{
			uv_cond_init(&condition_);
		}

		void notify_once()
		{
			uv_cond_signal(&condition_);
		}
		void notify_all()
		{
			uv_cond_broadcast(&condition_);
		}
		void wait(Mutex& mutex)
		{
			uv_cond_wait(&condition_, &mutex.mutex_);
		}


	private:
		uv_cond_t	condition_;
	};
	//#endif
	template<typename lt>
	class AutoLock :public MemObj
	{
	public:
		__inline AutoLock(lt* plock) :lock_(plock)
		{
			lock_->lock();
		}
		__inline ~AutoLock()
		{
			lock_->unlock();
		}
	private:
		lt*	lock_;
	};


	class CORE_API Thread :public MemObj
	{
	public:
		typedef Delegate1<void*>		ThreadFun;
		//typedef Functor<void,TYPELIST_1(void*)> ThreadFun;
		Thread(ThreadFun fun, void* thread_param) :function_(fun), param_(thread_param) {
			try
			{
				Assert(createThread());
			}
			catch (...)
			{
			}

		}
		~Thread()
		{
			//wait for finished
			Join();
		}
		RETURN_CODE							Join();
	private:

		bool								createThread();
		ThreadFun							function_;
		void*								param_;
#ifdef TNG_OS_FAMILY_WINDOWS
		static  u32			__stdcall		_threadfun(void* p)
		{
			Thread* t = (Thread*)p;
			t->function_(t->param_);
			return RETURN_SUCCESS;
		}
#elif defined(EMCC)
		static void			_threadfun(char *data, int size, void *arg)
		{
			Thread* t = (Thread*)data;
			t->function_(t->param_);
			return;
		}
#else
		static void*			_threadfun(void* p)
		{
			Thread* t = (Thread*)p;
			t->function_(t->param_);
			return NULL;
		}
#endif
		private_::tng_thread thread_;
	};
#include "atomicfun.h"
}
