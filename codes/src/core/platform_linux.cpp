#include "core/arch_detect.h"
#include "core/log.h"
#if TNG_OS_FAMILY_UNIX
#include "graphics/graphics.h"
namespace tng{
	void GFXService::ShowKeyboard(bool show)
	{
		Log::GetLog()->Printf(Log::DBG_CHN, "not support for linux");
	}
}
#endif // TNG_OS_FAMILY_UNIX
