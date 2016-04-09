#include "engine/event.h"
#include "engine/event_dispatcher.h"

namespace tng
{
	void EventListener::Connect(EventDispatcher* pDisp, EventKey eventKey)
	{
		pDisp->Connect(eventKey,this);
	}
	void EventListener::Disconnect(EventDispatcher* pDisp, EventKey eventKey)
	{
		pDisp->Disconnect(eventKey, this);
	}
	void EventListener::DisconnectAll(EventDispatcher* pDisp)
	{
		pDisp->Disconnect(this);
	}
}