#pragma once
#include "core/os.h"
#include "engine/engine_macros.h"
#include "core/refobject.h"
#include "uv/include/uv.h"
#include "core/threadfun.h"
#include <unordered_map>

namespace tng
{
	/**
		Loop
		One Loop is one thread
	*/
	class Service;
	class ENGINE_API Loop:public RefCountedObject
	{
	public:
		typedef std::unordered_map<string, Service*>	ServiceMap;
		enum RUN_VALUE
		{
			NORMAL,
			EXIT,
		};
		Loop();
		virtual ~Loop();
		virtual bool		Create();
		virtual void		Destory();
		virtual void		Start();

		void				DestoryNextFrame(){destory_ = true;}
		void				WaitThreadEnd();
		void				Update(void*);
		void				AddService(const string& service_name, Service* service);
		void				RemoveService(const string& service);
		Service*			GetService(const string& service);

	protected:
		tick_t						lastUpdateTime_;
		void						createThread();
		Thread*						work_thread_;
		AtomicCounter				destory_;
		ServiceMap					service_map_;
		Mutex						service_lock_;
		std::vector<string>	deletelist_;
	};
	/**
		main thread
	*/
	class ENGINE_API DefaultLoop:public Loop
	{
	public:
		virtual void	Start();
		//virtual	bool	Create();
	};
}