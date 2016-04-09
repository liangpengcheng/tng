#pragma once

#include "event.h"
#include "core/object.h"
#include <list>
#include <unordered_map>
namespace tng
{
	class ENGINE_API EventDispatcher :public RefCountedObject
	{
	public:
		EventDispatcher();
		~EventDispatcher();

		/***
			send an event to system 
			@param pEvent the event
			@param pListener the event handler set to NULL means all listener can receive this event
		*/
		void			SendEvent(Event* pEvent,EventListener* pListener);
		void			Connect(EventKey eventKey,EventListener* pListener);
		void			Disconnect(EventListener* pListener);
		void			Disconnect(EventKey key, EventListener* pListener);
		void			Update(f32 deltaTime);

		typedef std::list<EventListener*>				ListenerList;
		typedef	std::unordered_map<EventKey, ListenerList>		ListenerMap;
		struct EventPair
		{
			Event*			event_;
			EventListener*	listener_;
		};
	protected:
		void			_processEvent(Event* pEvent, EventListener* pListener);
		ListenerMap										listener_map_;
		std::list<EventPair>								event_list_;
		Mutex				event_lock_;
	};
}