#ifndef __TNG_LOG_H__
#define __TNG_LOG_H__
#include "core/core_macros.h"
#include "core/types.h"
#include <string>
#include <iostream>
namespace tng
{
	class CORE_API Log
	{
	public:
		Log();
		virtual ~Log();
		enum CHANNEL
		{
			WARNING_CHN = BIT(0),
			ERROR_CHN = BIT(1),
			SYS_CHN = BIT(2),
			DBG_CHN = BIT(3),
		};
		void			Printf(Log::CHANNEL ch, const char* fmt, ...);

		static void		EnableChannel(Log::CHANNEL ch);
		static void		DisableChannel(Log::CHANNEL ch);
		static Log*		GetLog(){ return global_log_; }
		static void		SetLog(Log* l){ global_log_ = l; }

		static u32		log_channel_mask_;

	protected:
		virtual void	Output(const char* msg, Log::CHANNEL ch) = 0;
		static Log*		global_log_;
	};

	class CORE_API ConsoleLog :public Log
	{
	public:
		ConsoleLog() :_str(
#ifdef TNG_OS_FAMILY_WINDOWS
			std::wclog
#else
			std::clog
#endif
			){};
		~ConsoleLog();
	protected:
		virtual void	Output(const char* msg, Log::CHANNEL ch);
#ifdef TNG_OS_FAMILY_WINDOWS
		std::wostream& _str;
#else
		std::ostream& _str;
#endif
	};
}


#endif