#pragma once
#include "string"
#include "core/stringfun.h"
#include "types.h"
#include "core/variant-internel.h"
#include "hgl/Str.h"
#include "core/delegate.h"
#include "core/math.h"
#include "core/stl_rebind.h"
#include "core/memobj.h"
#include "core/smartptr.h"
namespace tng
{

	template <typename T>
	__inline string ToString(const T& t)
	{
		return "unknown";
	}

	template <>
	__inline string	ToString<s32>(const s32& data){
		return tng::str(tng::format("%d") % data);
	}

	template <>
	__inline string	ToString<bool>(const bool& data){
		return data ? "true" : "false";
	}

	template <>
	__inline string	ToString<u32>(const u32& data){
		return tng::str(tng::format("%d") % data);
	}

	template <>
	__inline string	ToString<u16>(const u16& data){
		return tng::str(tng::format("%d") % data);
	}

	template <>
	__inline string	ToString<s16>(const s16& data){
		return tng::str(tng::format("%d") % data);
	}

	template <>
	__inline string	ToString<string>(const string& data){
		return data;
	}

	template <>
	__inline string	ToString<float2>(const float2& data){
		return tng::str(tng::format("%f,%f,%f")
			% data.x
			%data.y);
	}


	template <>
	__inline string	ToString<float3>(const float3& data){
		return tng::str(tng::format("%f,%f,%f")
			% data.x
			%data.y
			%data.z);
	}

	template <>
	__inline string	ToString<float4>(const float4& data){
		return tng::str(tng::format("%f,%f,%f,%f")
			% data.x
			%data.y
			%data.z
			%data.w);
	}

	template<typename T>
	__inline T FromString(const string& str)
	{
		return T();
	}

	template<>
	__inline s32 FromString<s32>(const string& str)
	{
		s32 ret;
		hgl::stoi(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline u32 FromString<u32>(const string& str)
	{
		u32 ret;
		hgl::stou(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline u16 FromString<u16>(const string& str)
	{
		u16 ret;
		hgl::stoi(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline s16 FromString<s16>(const string& str)
	{
		s16 ret;
		hgl::stoi(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline f32 FromString<f32>(const string& str)
	{
		f32 ret;
		hgl::etof(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline f64 FromString<f64>(const string& str)
	{
		f64 ret;
		hgl::etof(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline bool FromString<bool>(const string& str)
	{
		bool ret;
		hgl::stob(str.c_str(), ret);
		return ret;
	}

	template<>
	__inline float2 FromString<float2>(const string& str)
	{
		float2 ret;

		size_t pos = str.find_first_of(',');
		string xstr = str.substr(0, pos);
		string ystr = str.substr(pos + 1, str.length());

		ret.x = FromString<float>(xstr);
		ret.y = FromString<float>(ystr);

		return ret;
	}


	template<>
	__inline float3 FromString<float3>(const string& str)
	{
		float3 ret;

		size_t pos = str.find_first_of(',');
		string xstr = str.substr(0, pos);
		string rest = str.substr(pos + 1, str.length());

		pos = rest.find_first_of(',');
		string ystr = rest.substr(0, pos);
		string zstr = rest.substr(pos + 1, rest.length());

		ret.x = FromString<float>(xstr);
		ret.y = FromString<float>(ystr);
		ret.z = FromString<float>(zstr);
		return ret;
	}


	template<>
	__inline float4 FromString<float4>(const string& str)
	{
		float4 ret;

		size_t pos = str.find_first_of(',');
		string xstr = str.substr(0, pos);
		string rest = str.substr(pos + 1, str.length());

		pos = rest.find_first_of(',');
		string ystr = rest.substr(0, pos);
		rest = rest.substr(pos + 1, rest.length());

		pos = rest.find_first_of(',');
		string zstr = rest.substr(0, pos);
		rest = rest.substr(pos + 1, rest.length());

		ret.x = FromString<float>(xstr);
		ret.y = FromString<float>(ystr);
		ret.z = FromString<float>(zstr);
		ret.w = FromString<float>(rest);

		return ret;
	}
	template<>
	__inline string FromString<string>(const string& str)
	{
		return str;
	}



	class  CORE_API BaseAcesser :public MemObj
	{
	public:
		virtual ~BaseAcesser(){}
		virtual string	Get(const void* baseAddress) = 0;
		virtual void	Set(void* baseAddress, const string& value) = 0;
	};

	template<typename ClassName, typename PorpertyType>
	class Accesser :public BaseAcesser
	{
	public:
		typedef void			(ClassName::*setter)(const PorpertyType&);
		typedef PorpertyType(ClassName::*getter)(void)const;
		typedef void (ClassName::*setter2)(PorpertyType);

		Accesser(getter getFun, setter setFun)
		{
			getter_ = getFun;
			setter_ = setFun;
		}
		Accesser(getter getFun, setter2 setFun)
		{
			getter_ = getFun;
			setter_ = reinterpret_cast<setter>(setFun);
		}
		string	Get(const void* baseAddress)
		{
			Delegate0<PorpertyType> fun((ClassName*)baseAddress, getter_);
			return ToString(fun());
		}
		void	Set(void* baseAddress, const string& value)
		{
			Delegate1<const PorpertyType&> fun((ClassName*)baseAddress, setter_);
			fun(FromString<PorpertyType>(value));
		}

		getter	getter_;
		setter	setter_;
	};
	typedef SmartPtr<BaseAcesser> AccesserPtr;
}