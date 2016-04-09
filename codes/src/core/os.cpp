

#include "core/os.h"
#include "core/stringfun.h"

extern "C"
{
#ifndef _WIN32
#include <sys/utsname.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#else

#endif

#include "uv/include/uv.h"
#ifdef TNG_OS_FAMILY_WINDOWS
	extern void uv_winapi_init();
#endif

}


namespace tng
{
#ifdef ANDROID
	android_app* OS::state__ = NULL;
#endif
	void OS::InitPlatform()
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		uv_winapi_init();
#endif
	}
	u64 OS::GetThreadID()
	{
#ifdef _WIN32
		return (unsigned long)GetCurrentThreadId();
#else
		return (unsigned long)pthread_self();
#endif
	}
	void OS::Sleep(f32 second)
	{
		Clock::Sleep(u32(second*1000.0f));
	}
	string		OS::GetCpuString()
	{
		return GetProcessorBrandName().c_str();
	}
	string		OS::GetOSString()
	{
		return GetOSDisplayString().c_str();
	}


	///get free memory in bytes
	u64				OS::GetFreeMemory()
	{
		return uv_get_free_memory();
	}
	///get total memory in bytes
	u64				OS::GetTotalMemory()
	{
		return GetTotalSystemPhysicalMemory();
	}


}

