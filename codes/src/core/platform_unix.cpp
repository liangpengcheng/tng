#include "core/arch_detect.h"
#include "core/log.h"
#include "core/core_macros.h"
#ifdef TNG_OS_FAMILY_UNIX
#include "core/os.h"
namespace tng{
	void OS::ShowKeyboard(bool show)
	{
		Log::GetLog()->Printf(Log::DBG_CHN, "not support for unix");
	}
	//copy from gameplay
	bool OS::LaunchURL(const char* url)
	{
		if (url == NULL || *url == '\0')
			return false;

		int len = strlen(url);

		char* cmd = new char[11 + len];
		sprintf(cmd, "xdg-open %s", url);
		int r = system(cmd);
		SAFE_DELETE_ARRAY(cmd);
		return (r == 0);
	}
}
#endif // 
