#include "core/object.h"
namespace tng
{
	//DynamicFactory<Object,string,Mutex>	Object::dynamic_factory_static;
	void Object::Set(const string& key, const string& value)
	{
		AccesserPtr p = Object::GetPropertySet()->GetProperty(key);
		if (p)
		{
			p->Set(this, value);
		}
		else
		{
			Log::GetLog()->Printf(Log::WARNING_CHN, "can't find key %s", key.c_str());
		}
	}
	string Object::Get(const string& key)const
	{
		AccesserPtr p = Object::GetPropertySet()->GetProperty(key);
		if (p)
		{
			return p->Get(this);
		}
		else
		{
			return "";
		}
	}

	void Object::LoadFromJson(const JsonObject& obj)
	{
		for (PropertySet::PropertyMap::iterator it = GetPropertySet()->Begin();
			it != GetPropertySet()->End();
			it++)
		{
			if (obj.HasMember(it->first.c_str()))
			{
				Set(it->first, obj[it->first.c_str()].GetString());
			}
		}
	}
	void Object::SaveToJson(JsonObject& json)const
	{
		for (PropertySet::PropertyMap::iterator it = GetPropertySet()->Begin();
			it != GetPropertySet()->End();
			it++)
		{
			json[it->first.c_str()] = Get(it->first).c_str();
		}

	}

	START_CLASS(Object)
		END_CLASS
}