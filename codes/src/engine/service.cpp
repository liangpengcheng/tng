#include "engine/service.h"
#include "core/os.h"
namespace tng
{
	Service::~Service()
	{
	}

	UpdateableService::UpdateableService():lastUpdateTime_(0)
	{
		SetTimer(0.01f);
	}
	UpdateableService::~UpdateableService()
	{

	}

	bool UpdateableService::Initialize(Loop* loop)
	{
		initialized_ = true;
		return true;
	}
	void UpdateableService::SetTimer(f32 deltaTime)
	{
		update_time_ = deltaTime;
		lastUpdateTime_= Clock::Tick();
	}

	void UpdateableService::LoadFromConfig(JsonObject& doc)
	{
		SetTimer(f32(doc["timer"].GetDouble()));
	}

	
	
	START_SUB_ABSTRACT_CLASS(Service,Object)
	END_CLASS
	


	START_SUB_CLASS(UpdateableService,Service)
	END_CLASS
}