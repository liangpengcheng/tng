#include "core/arch_detect.h"
#include "core/log.h"
#include "core/os.h"
#include "core/stl_rebind.h"
#include "core/stringfun.h"
#ifdef TNG_OS_FAMILY_WINDOWS
namespace tng{
	void OS::ShowKeyboard(bool show)
	{
		Log::GetLog()->Printf(Log::DBG_CHN, "not support for windows");
	}
	//copy from gameplay
	bool OS::LaunchURL(const char* url)
	{
		if (url == NULL || *url == '\0')
			return false;

		// Success when result code > 32
		int len = MultiByteToWideChar(CP_ACP, 0, url, -1, NULL, 0);
		w16string out;
		ToWideString(url, out);
		int r = (int)ShellExecuteW(NULL, NULL, out.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return (r > 32);
	}

}
#endif // TNG_OS_FAMILY_WINDOWS
