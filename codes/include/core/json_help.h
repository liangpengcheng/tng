#ifndef TNG_JSON_HELP
#define TNG_JSON_HELP
#include "types.h"
#include "variant.h"
namespace tng
{
	template<typename T>
	__inline void GetValue(T& t,const JsonObject& jobj,const string& key)
	{
		Assert(0)
	}

	template <>
	__inline void GetValue<bool>(bool& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = jobj[key.c_str()].GetBool();
		}
	}

	template <>
	__inline void GetValue<int>(int& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = jobj[key.c_str()].GetInt();
		}
	}

	template <>
	__inline void GetValue<u32>(u32& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = jobj[key.c_str()].GetUint();
		}
	}

	template <>
	__inline void GetValue<u64>(u64& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = jobj[key.c_str()].GetUint64();
		}
	}

	template <>
	__inline void GetValue<float>(float& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = (float)jobj[key.c_str()].GetDouble();
		}
	}

	template <>
	__inline void GetValue<s64>(s64& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = jobj[key.c_str()].GetInt64();
		}
	}
	template <>
	__inline void GetValue<string>(string& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			t = jobj[key.c_str()].GetString();
		}
	}

	template <>
	__inline void GetValue<float4>(float4& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			string str = jobj[key.c_str()].GetString();
			t = FromString<float4>(str);
		}
	}
	template <>
	__inline void GetValue<float3>(float3& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			string str = jobj[key.c_str()].GetString();
			t = FromString<float3>(str);
		}
	}
	template <>
	__inline void GetValue<float2>(float2& t,const JsonObject& jobj,const string& key)
	{
		if (jobj.HasMember(key.c_str()))
		{
			string str = jobj[key.c_str()].GetString();
			t = FromString<float2>(str);
		}
	}
}
#endif