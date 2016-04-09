#pragma once
#include "exception.h"
#include <string>
#include "stringfun.h"
#include "log.h"
#include "MathGeoLib/Math/Callstack.h"
#define Assert(x)	if(!(x)){\
	Log::GetLog()->Printf(Log::ERROR_CHN, "%s,%d callstacks:\n%s", __FILE__, __LINE__, GetCallstack().c_str()); \
	throw tng::AssertionViolationException(tng::Format("%s,%d", __FILE__, __LINE__), 0); }
#define CheckPtr(p) if(!(p)){\
	Log::GetLog()->Printf(Log::ERROR_CHN, "%s,%d,callstacks:\n%s", __FILE__, __LINE__, GetCallstack().c_str()); \
	throw tng::NullPointerException(tng::Format("%s,%d", __FILE__, __LINE__)); }

namespace tng
{

}