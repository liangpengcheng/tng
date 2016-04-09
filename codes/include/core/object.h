#pragma once
#include "core/refobject.h"
#include "core/class_desc.h"
#include "core/dynamic_factory.h"
#include "core/stl_rebind.h"
namespace tng
{
	class CORE_API Object :public RefCountedObject
	{
	public:
		ENGINE_CLASS_DEF
#ifdef USE_RTTI
			template<class _Obj>
		__inline bool				IsType()const{ return IsType(_Obj::GetRtti()); }
		template<class _Obj>
		bool						IsDerivedFrom()const{ return IsDerivedFrom(_Obj::GetRtti()); }
		template<typename T>
		__inline const T*			DynamicCast()const
		{
			if (IsDerivedFrom<T>())
				return (T*)(this);
			else
				return NULL;
		}
		template<typename T>
		__inline T*					DynamicCast()
		{
			if (IsDerivedFrom<T>())
				return (T*)(this);
			else
				return NULL;
		}
#else
			template<typename T>
		__inline const T*			DynamicCast()const
		{
			return (T*)(this);
		}
		template<typename T>
		__inline T*					DynamicCast()
		{

			return (T*)(this);
		}

		virtual void			LoadFromJson(const JsonObject& json);
		virtual void			SaveToJson(JsonObject& json)const;
#endif
		void Set(const string& key, const string& value);
		string Get(const string& key)const;
		static Object* CreateObject(const string& cname){ return GetDynamicFactory().CreateObject(cname); }
		static DynamicFactory<Object, string, Mutex>& GetDynamicFactory(){
			static DynamicFactory<Object, string, Mutex>		dynamic_factory_static;
			return dynamic_factory_static;
		}
#ifdef USE_RTTI
		static vector<ClassDesc*>&	GetAllClasses()
		{
			static vector<ClassDesc*> all_classes_static;
			return all_classes_static;
		}
#endif // USE_RTTI
	};
}