#pragma once
#ifndef _WIN32
#include <stdio.h>
#endif
#include "core/arch_detect.h"
#ifdef TNG_ANDROID
#include "android_native_app_glue.h"
#include "android/native_window.h"
#endif // TNG_ANDROID
#include "stl_rebind.h"
#include "core/types.h"
#include "core/core_macros.h"
#include "MathGeoLib/Time/Clock.h"
namespace tng
{
	namespace core
	{
		CORE_FUNC_API void InitCore();
	}
	class CORE_API OS
	{
	public:
		static string			GetCpuString();
		static string			GetOSString();

		///get free memory in bytes
		static u64				GetFreeMemory();
		static u64				GetTotalMemory();
		static u64				GetThreadID();
		static void				Sleep(f32 second);
		static void				InitPlatform();
#ifdef TNG_OS_FAMILY_WINDOWS
#endif // TNG_OS_FAMILY_WINDOWS

		// show keyboard 
		// available for android ios wp
		static void				ShowKeyboard(bool show);
		static bool				LaunchURL(const char *url);
#ifdef TNG_ANDROID
	public:
		static								android_app* state__;
#endif // TNG_ANDROID
	};
}
