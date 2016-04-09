#pragma once
#include "core/object.h"
#include "engine_macros.h"
namespace tng
{
	enum EEngineEvent
	{
		EVENT_INPUT = 1,
	};
	enum EInputState
	{
		ELButton = BIT(0),
		ERButton = BIT(1),
		EMButton = BIT(2),
	};
	typedef u32 EventKey;
	/**
		the param pass to event handler
	*/
	class ENGINE_API Event:public RefCountedObject
	{
	public:
		Event() :discard_(0), delay_(.0f){};
		virtual EventKey GetEventKey() = 0;
		///when discard_ set to true ,this event will not pass to next handler
		int							discard_;
		float						delay_;
	};

	/**
		EventListenere is handler of event,if you want to listen some event from specific EventDispatcher call connect
		@remark when you Connect some event you must call Disconnect before destroy this EventListener
	*/
	class ENGINE_API EventListener
	{
	public:
		/**
			start listen eventKey from pDisp
		*/
		void			Connect(class EventDispatcher* pDisp, EventKey eventKey);
		/**
			stop listen eventKey from pDisp
		*/
		void			Disconnect(class EventDispatcher* pDisp, EventKey eventKey);
		/**
			stop listen all events from pDisp
		*/
		void			DisconnectAll(class EventDispatcher* pDisp);

		virtual void	OnEvent(Event* pEvent){};

	};
}