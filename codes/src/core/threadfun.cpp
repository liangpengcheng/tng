#include "core/threadfun.h"
#if defined(EMCC)
#include <emscripten.h>
#include "core/os.h"
#endif
namespace tng
{

	bool Thread::createThread()
	{

#ifdef TNG_OS_FAMILY_WINDOWS
		thread_ = (HANDLE)_beginthreadex(NULL, 0, Thread::_threadfun, this, 0, NULL);
		if (thread_ == 0) {
#elif defined(EMCC)
		thread_ = emscripten_create_worker("worker.js");
		std::string name= str(format("%d")%int(this));
		emscripten_call_worker(thread_, name.c_str(), (char*)this, sizeof(this), Thread::_threadfun, NULL);
		if(emscripten_get_worker_queue_size(thread_) != 1){
#else
		if (pthread_create(&thread_, NULL, Thread::_threadfun, this)) {
#endif
			return false;
		}

		return true;
	}
	RETURN_CODE Thread::Join()
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		if (WaitForSingleObject(thread_, INFINITE))
			return RETURN_TIMEOUT;
		else {
			CloseHandle(thread_);
			thread_ = 0;
			return RETURN_SUCCESS;
		}
#elif defined(EMCC)
		while(emscripten_get_worker_queue_size(thread_) != 0)
		{
			OS::Sleep(0.1f);
		}
		return RETURN_SUCCESS;
#else
		if (pthread_join(thread_, NULL))
			return RETURN_TIMEOUT;
		else
			return RETURN_SUCCESS;
#endif
	}
}