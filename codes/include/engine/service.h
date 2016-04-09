#pragma once
#include "engine/engine_macros.h"
#include "core/object.h"
#include "core/autoptr.h"
#include "engine/threadloop.h"
#include <list>
namespace tng
{
	
	class ENGINE_API Service:public Object
	{
	public:
		ENGINE_CLASS_DEF
		Service():initialized_(false){}
		virtual ~Service();
		/**
			Initialize the service 
			this function will be called automatic in work thread
			@param loop this service will run on loop thread
		*/
		virtual bool			Initialize(Loop* loop)						= 0;
		virtual void			OnDestory()									= 0;
		virtual void			LoadFromConfig(JsonObject& doc)				= 0;
		virtual void			_update(f32 delta_time)						= 0;
		bool					Initialized()const{return initialized_;}
		bool					initialized_;
	};

	
	

	class ENGINE_API Command:public MemObj
	{
	public:
		virtual ~Command(){};
		virtual bool Do() = 0;
	};

	class ENGINE_API UpdateableService:public Service

	{
	public:
		ENGINE_CLASS_DEF
		UpdateableService();
		virtual ~UpdateableService();
		
		
		bool					Initialize(Loop* loop);
		virtual void			LoadFromConfig(JsonObject& doc);
		
		void					SetTimer(f32 deltaTime);
		//call in thread when thread exit
		virtual void			OnDestory(){}
		virtual void			_update(f32 deltaTime){};
	
	private:

	public:
		f32						update_time_;
		tick_t					lastUpdateTime_;
		
	};



	
	

	typedef AutoPtr<Service>	PService;
}