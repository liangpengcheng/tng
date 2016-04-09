#include "engine/event_dispatcher.h"

namespace tng
{
	EventDispatcher::EventDispatcher(){}
	EventDispatcher::~EventDispatcher(){}

	void EventDispatcher::SendEvent(Event* pEvent, EventListener* pListener)
	{
		AutoLock<Mutex> al(&event_lock_);
		//确保在同一线程里面，所以无论如何都要插入到列表
// 		if (pEvent->delay_<=.0f)
// 		{
// 			_processEvent(pEvent, pListener);
// 		}
// 		else
		{
			EventPair ep;
			ep.event_ = pEvent;
			ep.listener_ = pListener;
			pEvent->Duplicate();
			event_list_.push_back(ep);
		}
	}
	void EventDispatcher::Update(f32 deltaTime)
	{
		AutoLock<Mutex> al(&event_lock_);
		for (std::list<EventPair>::iterator it = event_list_.begin();
			it != event_list_.end();)
		{
			if ((*it).event_->delay_ <= .0f)
			{
				_processEvent((*it).event_, (*it).listener_);
				(*it).event_->Release();
				it = event_list_.erase(it);
			}
			else
			{
				(*it).event_->delay_ -= deltaTime;
				it++;
			}
		}
	}
	void EventDispatcher::_processEvent(Event* pEvent, EventListener* pListener)
	{
		
		if (pListener)
		{
			pListener->OnEvent(pEvent);
		}
		else
		{
			EventKey key = pEvent->GetEventKey();
			ListenerMap::iterator listenerIt = listener_map_.find(key);
			if (listenerIt != listener_map_.end())
			{
				for (ListenerList::iterator itbegin = listenerIt->second.begin();
					itbegin != listenerIt->second.end(); itbegin++)
				{
					(*itbegin)->OnEvent(pEvent);
				}
			}
		}
		
	}

	void EventDispatcher::Connect(EventKey eventKey,EventListener* pListener)
	{
		AutoLock<Mutex> al(&event_lock_);
		listener_map_[eventKey].push_back(pListener);
	}

	void EventDispatcher::Disconnect(EventKey key, EventListener* pListener)
	{
		AutoLock<Mutex> al(&event_lock_);
		ListenerMap::iterator listenerIt= listener_map_.find(key);
		if (listenerIt != listener_map_.end())
		{
			for (ListenerList::iterator itbegin = listenerIt->second.begin();
				itbegin != listenerIt->second.end();)
			{
				if ((*itbegin) == pListener)
				{
					itbegin = listenerIt->second.erase(itbegin);
				}
				else
					itbegin++;
			}
		}
	}
	void EventDispatcher::Disconnect(EventListener* pListener)
	{
		AutoLock<Mutex> al(&event_lock_);
		for (ListenerMap::iterator listenerIt = listener_map_.begin();
			listenerIt != listener_map_.end();
			listenerIt++)
		{
			for (ListenerList::iterator itbegin = listenerIt->second.begin();
				itbegin != listenerIt->second.end();)
			{
				if ((*itbegin) == pListener)
				{
					itbegin = listenerIt->second.erase(itbegin);
				}
				else
					itbegin++;
			}
		}
	}
}