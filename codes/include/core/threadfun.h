#pragma once
#include "core_exports.h"
#include "config.h"
#include "types.h"
#include "returncode.h"
#include "core/delegate.h"
#include "core/memobj.h"

#if TNG_OS == TNG_OS_MAC_OS_X
#include <libkern/OSAtomic.h>
#endif

#ifdef TNG_OS_FAMILY_WINDOWS
#include <windows.h>
#elif defined(EMCC)
#include <emscripten.h>
#else
#include <pthread.h>
#include <semaphore.h>
#if TNG_OS == TNG_OS_MAC_OS_X
#include <mach/mach.h>
#endif
#endif

namespace tng
{
	namespace private_
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		typedef HANDLE tng_thread;
		typedef HANDLE tng_sem;
		typedef CRITICAL_SECTION tng_mutex;
		typedef CONDITION_VARIABLE tng_cond;
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
		typedef pthread_cond_t tng_cond;
		typedef pthread_rwlock_t tng_rwlock;
#endif
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
#ifdef TNG_OS_FAMILY_WINDOWS
			InitializeCriticalSection(&mutex_);
#else
			pthread_mutex_init(&mutex_, NULL);
#endif
		}
		~Mutex() {
#ifdef TNG_OS_FAMILY_WINDOWS
			DeleteCriticalSection(&mutex_);
#else
			pthread_mutex_destroy(&mutex_);
#endif
		}

		void lock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			EnterCriticalSection(&mutex_);
#else
			pthread_mutex_lock(&mutex_);
#endif
		}
		void unlock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			LeaveCriticalSection(&mutex_);
#else
			pthread_mutex_unlock(&mutex_);
#endif
		}
	private:
		private_::tng_mutex mutex_;
		friend class Condition;
	};
	//#if !defined(NO_LIBUV)&&!defined(EMCC)
	class CORE_API RWLock :public MemObj
	{
	public:
		RWLock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			InitializeSRWLock(&rwlock_);
#else
			pthread_rwlock_init(&rwlock_, NULL);
#endif
		}
		~RWLock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			// SRWLock doesn't need destruction
#else
			pthread_rwlock_destroy(&rwlock_);
#endif
		}
		void rdlock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			AcquireSRWLockShared(&rwlock_);
#else
			pthread_rwlock_rdlock(&rwlock_);
#endif
		}
		void rdunlock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			ReleaseSRWLockShared(&rwlock_);
#else
			pthread_rwlock_unlock(&rwlock_);
#endif
		}
		void wrlock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			AcquireSRWLockExclusive(&rwlock_);
#else
			pthread_rwlock_wrlock(&rwlock_);
#endif
		}
		void wrunlock() {
#ifdef TNG_OS_FAMILY_WINDOWS
			ReleaseSRWLockExclusive(&rwlock_);
#else
			pthread_rwlock_unlock(&rwlock_);
#endif
		}
	private:
#ifdef TNG_OS_FAMILY_WINDOWS
		SRWLOCK rwlock_;
#else
		private_::tng_rwlock rwlock_;
#endif
	};

	class CORE_API Semaphore :public MemObj
	{
	public:
		Semaphore(u32 val = 0) {
#ifdef TNG_OS_FAMILY_WINDOWS
			semaphore_ = CreateSemaphore(NULL, val, INT_MAX, NULL);
#elif TNG_OS == TNG_OS_MAC_OS_X
			semaphore_create(mach_task_self(), &semaphore_, SYNC_POLICY_FIFO, val);
#else
			sem_init(&semaphore_, 0, val);
#endif
		}
		~Semaphore() {
#ifdef TNG_OS_FAMILY_WINDOWS
			CloseHandle(semaphore_);
#elif TNG_OS == TNG_OS_MAC_OS_X
			semaphore_destroy(mach_task_self(), semaphore_);
#else
			sem_destroy(&semaphore_);
#endif
		}

		void wait() {
#ifdef TNG_OS_FAMILY_WINDOWS
			WaitForSingleObject(semaphore_, INFINITE);
#elif TNG_OS == TNG_OS_MAC_OS_X
			semaphore_wait(semaphore_);
#else
			sem_wait(&semaphore_);
#endif
		}
		s32  trywait() {
#ifdef TNG_OS_FAMILY_WINDOWS
			return WaitForSingleObject(semaphore_, 0) == WAIT_OBJECT_0 ? 0 : -1;
#elif TNG_OS == TNG_OS_MAC_OS_X
			mach_timespec_t interval = {0, 0};
			return semaphore_timedwait(semaphore_, interval) == KERN_SUCCESS ? 0 : -1;
#else
			return sem_trywait(&semaphore_);
#endif
		}
		void post() {
#ifdef TNG_OS_FAMILY_WINDOWS
			ReleaseSemaphore(semaphore_, 1, NULL);
#elif TNG_OS == TNG_OS_MAC_OS_X
			semaphore_signal(semaphore_);
#else
			sem_post(&semaphore_);
#endif
		}

	private:
		private_::tng_sem semaphore_;
	};

	class CORE_API Condition :public MemObj
	{
	public:
		Condition()
		{
#ifdef TNG_OS_FAMILY_WINDOWS
			InitializeConditionVariable(&condition_);
#else
			pthread_cond_init(&condition_, NULL);
#endif
		}

		void notify_once()
		{
#ifdef TNG_OS_FAMILY_WINDOWS
			WakeConditionVariable(&condition_);
#else
			pthread_cond_signal(&condition_);
#endif
		}
		void notify_all()
		{
#ifdef TNG_OS_FAMILY_WINDOWS
			WakeAllConditionVariable(&condition_);
#else
			pthread_cond_broadcast(&condition_);
#endif
		}
		void wait(Mutex& mutex)
		{
#ifdef TNG_OS_FAMILY_WINDOWS
			SleepConditionVariableCS(&condition_, &mutex.mutex_, INFINITE);
#else
			pthread_cond_wait(&condition_, &mutex.mutex_);
#endif
		}


	private:
		private_::tng_cond	condition_;
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
				createThread();
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
