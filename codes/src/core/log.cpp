#include "core/log.h"
#include "core/stringfun.h"
#include "core/os.h"
#include "MathGeoLib/Time/Clock.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "MathGeoLib/Math/Callstack.h"
#if defined(ANDROID)
/// This will require you to pass '-llog' on the command line to link against the Android logging libraries.
#include <android/log.h>
#endif
namespace tng
{
	u32 Log::log_channel_mask_ = Log::WARNING_CHN | Log::DBG_CHN | Log::SYS_CHN | Log::ERROR_CHN;
	Log* Log::global_log_ = NULL;
	Log::Log(){}
	Log::~Log(){}
	void Log::EnableChannel(Log::CHANNEL ch)
	{
		log_channel_mask_ |= ch;
	}
	void Log::DisableChannel(Log::CHANNEL ch)
	{
		log_channel_mask_ &= ~u32(ch);
	}

	void Log::Printf(Log::CHANNEL ch, const char* fmt, ...){
		if (ch&log_channel_mask_)
		{
			int n, size = 128;
			char *p;
			va_list ap;
			if ((p = (char*)malloc(size)) == NULL)
				return;
			while (1) {
				/* Try to print in the allocated space. */
				va_start(ap, fmt);
				n = vsnprintf(p, size, fmt, ap);
				va_end(ap);
				/* If that worked, return the string. */
				if (n > -1 && n < size)
					break;
				/* Else try again with more space. */
				if (n > -1)    /* glibc 2.1 */
					size = n + 1; /* precisely what is needed */
				else           /* glibc 2.0 */
					size *= 2;  /* twice the old size */
				char* oldp = p;
				if ((p = (char*)realloc(p, size)) == NULL)
				{
					free(oldp);
					break;
				}
			}

			const char* channelmsg;
			switch (ch)
			{
			case tng::Log::WARNING_CHN:
				channelmsg = "warning";
				break;
			case tng::Log::ERROR_CHN:
				channelmsg = "error";
				break;
			case tng::Log::SYS_CHN:
				channelmsg = "system";
				break;
			case tng::Log::DBG_CHN:
				channelmsg = "debug";
				break;
			default:
				break;
			}
#ifdef _DEBUG
			string opt = ch == tng::Log::ERROR_CHN ? str(format("[T:%x][%s] %s [call-stacks:%s]\n") % OS::GetThreadID() % channelmsg%p%GetCallstack().c_str()) :
				str(format("[T:%x][%s] %s\n") % OS::GetThreadID() % channelmsg%p);
#else
			string opt = str(format("[T:%x][%s] %s\n") %OS::GetThreadID()%channelmsg%p);
#endif
			Output(opt.c_str(), ch);
			free(p);
		}

	}
#ifdef TNG_OS_FAMILY_UNIX
#define ANSI_COLOR_RED     "\33[1;31m"
#define ANSI_COLOR_GREEN   "\33[1;32m"
#define ANSI_COLOR_YELLOW  "\33[1;33m"
#define ANSI_COLOR_WITE    "\33[1;37m"
#elif TNG_OS_FAMILY_WINDOWS
	void set_console_color(unsigned short color_index)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
	}
#endif
	ConsoleLog::~ConsoleLog()
	{
#ifdef TNG_OS_FAMILY_UNIX
		_str<<ANSI_COLOR_WITE;
#elif defined(TNG_OS_FAMILY_WINDOWS)
		set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#endif
	}
	void ConsoleLog::Output(const char* msg, Log::CHANNEL ch)
	{
#ifdef TNG_ANDROID
		switch (ch)
		{
		case tng::Log::WARNING_CHN:
			(void)__android_log_print(ANDROID_LOG_WARN, "tng_native", msg);
			break;
		case tng::Log::ERROR_CHN:
			(void)__android_log_print(ANDROID_LOG_ERROR, "tng_native", msg);
			break;
		case tng::Log::SYS_CHN:
			(void)__android_log_print(ANDROID_LOG_INFO, "tng_native", msg);
			break;
		case tng::Log::DBG_CHN:
			(void)__android_log_print(ANDROID_LOG_INFO, "tng_native", msg);
			break;
		default:
			break;
		}
#elif defined(TNG_OS_FAMILY_UNIX)
		switch (ch)
		{
		case tng::Log::WARNING_CHN:
			_str<<ANSI_COLOR_YELLOW<<msg;
			break;
		case tng::Log::ERROR_CHN:
			_str<<ANSI_COLOR_RED<<msg;
			break;
		case tng::Log::SYS_CHN:
			_str<<ANSI_COLOR_WITE<<msg;
			break;
		case tng::Log::DBG_CHN:
			_str<<ANSI_COLOR_GREEN<<msg;
			break;
		default:
			break;
		}

#elif defined(TNG_OS_FAMILY_WINDOWS)
		switch (ch)
		{
		case tng::Log::WARNING_CHN:
			set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case tng::Log::ERROR_CHN:
			set_console_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;
		case tng::Log::SYS_CHN:
			set_console_color(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		case tng::Log::DBG_CHN:
			set_console_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		default:
			break;
		}
		w16string out;
		ToWideString(msg, out);
		_str << out;
#endif

	}
}