
#include "engine/engine.h"
#include "engine/service.h"
namespace tng
{
	namespace engine
	{
		void InitEngine()
		{
			Log::GetLog()->Printf(Log::SYS_CHN,OS::GetCpuString().c_str());
			Log::GetLog()->Printf(Log::SYS_CHN,OS::GetOSString().c_str());
			Log::GetLog()->Printf(Log::SYS_CHN,"all memory:%lld free memory:%lld",OS::GetTotalMemory(),OS::GetFreeMemory());
			static bool inited =false;
			if (!inited)
			{
				inited = true;
				core::InitCore();
				Service::InitStatic();
				UpdateableService::InitStatic();
			}
		}
		
	}
}