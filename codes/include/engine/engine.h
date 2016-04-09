#pragma once
#include "engine/engine_macros.h"
#include "core/refobject.h"
#include "core/autoptr.h"
#include "engine/threadloop.h"
#include "engine/service.h"
#include "core/filesystem.h"
#include "core/stl_rebind.h"
#include "core/signal.h"
namespace tng
{
	class ENGINE_API Engine:public RefCountedObject
	{
	public:
		Engine();
		~Engine();
		/**
			Create Engine instance
			@remark call this function only one time in your app.if you call it over one times will throw an exception
		*/
		static Engine*	Create();
		/**
			Destory Engine instance
		*/
		static void		Destory();
		/**
			Get Engine instance
		*/
		static Engine*	GetInstance(){return global_engine_.get();}


		/**
			Create a loop
			@param loopname loop name used to find a specific loop
		*/
		Loop*			CreateLoop(const string& loopname);

		/**
			Find a specific loop
		*/
		Loop*			GetLoop(const string& loopname);

		/**
			Destory a specific loop
		*/
		void			DestoryLoop(const string& loopname);

		/**
			Get the main-thread loop
		*/
		Loop*			GetDefaultLoop();

		/**
			Insert a loop into map
			if loopname has been exist it will throw an exception
		*/
		void			InsertLoop(const string& loopname,Loop* loop);

		/**
			Create a service
			@param classname class name
			@param servicename service name used to look up specific loop
			@param belongloop the thread this service belong to
		*/
		Service*		CreateService(const string& classname,const string& servicename,const string& belongloop);

		void			DestoryService(const string& servicename,const string& belongloop="DefaultLoop");

		Service*		GetService(const string& servicename, const string& belongloop = "DefaultLoop");

		/**
			run default loop
		*/
		void			MainLoop();

		enum State
		{
			Running = 0,
			Pause	= 1,
			Existing = 2,
		};
		typedef Signal2<State,State>::_Delegate OnStateChange; 
		typedef Signal2<State,State> OnStateChangeList;

		__inline bool			IsRunning(){
			return GetAppState() == Running;
		}

		__inline State			GetAppState(){return (State)(current_state_.value());}

		__inline void			OnExisting()
		{
			if (!state_change_callback_.Empty())
			{
				state_change_callback_((State)(current_state_.value()),Pause);
			}
			current_state_ = Existing;
			Log::GetLog()->Printf(Log::SYS_CHN,"app existing");
		}

		__inline void			OnPause(){
			if (!state_change_callback_.Empty())
			{
				state_change_callback_((State)(current_state_.value()),Pause);
			}
			current_state_ = Pause;
			Log::GetLog()->Printf(Log::SYS_CHN,"app paused");
		}
		__inline void			OnResume(){
			if (!state_change_callback_.Empty())
			{
				state_change_callback_((State)(current_state_.value()),Running);
			}
			current_state_ = Running;
			Log::GetLog()->Printf(Log::SYS_CHN,"app resumed");
		}
		__inline void	AddStateChangeCallback(OnStateChange cb)
		{
			state_change_callback_.Connect(cb);
		}
		__inline void RemoveStateChangeCallback(OnStateChange cb)
		{
			state_change_callback_.Disconnect(cb);
		}
		__inline string GetAppName(){
			return app_name_;
		}
		__inline void SetAppName(const string& name){
			app_name_ = name;
		}
		typedef unordered_map<string,Loop*>		LoopMap;
	private:
		static AutoPtr<Engine>			global_engine_;
		string							app_name_;
		LoopMap							loop_map_;
		AtomicCounter					current_state_;
		OnStateChangeList				state_change_callback_;
	};

	namespace engine
	{
		ENGINE_FUNC_API void InitEngine();
	}
}