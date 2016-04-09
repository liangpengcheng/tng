#pragma once
#include "engine/engine.h"
#include "engine/service.h"
#include "engine/networkservice.h"
using namespace tng;

TEST(engine, Engine) {
	Engine* pengine = Engine::Create();
	pengine->MainLoop();
	Engine::Destory();
}



#include "engine/event.h"
#include "engine/event_dispatcher.h"
#define EventTestKey 0
class EventTest :public Event
{
public:
	virtual EventKey GetEventKey(){ return EventTestKey; }
};
class EventListenerTest :public EventListener
{
public:
	EventListenerTest() :testValue(1){}
	void OnEvent(Event* pevent)
	{
		testValue = 0;
		ASSERT_EQ(pevent->GetEventKey(), EventTestKey);
	}
	int testValue ;
};


TEST(engine, Event) {
	EventDispatcher disp;
	EventListenerTest testListener;
	EventTest* pEvent = new EventTest;
	testListener.Connect(&disp, EventTestKey);
	disp.SendEvent(pEvent,NULL);
	disp.Update(100);
	testListener.Disconnect(&disp, EventTestKey);

	testListener.Connect(&disp, EventTestKey);
	pEvent->delay_ = 10;
	testListener.testValue = 1;
	disp.SendEvent(pEvent, &testListener);
	while (testListener.testValue)
	{
		disp.Update(1);
	}
	
	testListener.DisconnectAll(&disp);
	pEvent->Release();

}

