#pragma once
#include "core/core_exports.h"
#include "core/refobject.h"
#include "core/property.h"
#include "core/runtime_check.h"
#include <vector>
namespace tng
{
#ifdef USE_RTTI
	class ClassDesc
	{
	public:
		ClassDesc(){}
		~ClassDesc(){}
		///get class name
		__inline const string&	GetClassName()const{return str_class_name_;}
		__inline void SetClassName(const string& cname){str_class_name_ = cname;}


		///set father class
		__inline void				SetInheritClass(ClassDesc* pcd){
			inherit_class_ = pcd;
		}
		__inline const ClassDesc*	GetInherit()const{return inherit_class_;}
		///if this class is derived from pcd return true,if this class is pcd return true
		__inline bool				IsDerivedFrom(const ClassDesc* pcd)const{
			CheckPtr(pcd);
			if (this == pcd)
				return true;
			if(inherit_class_)
				return inherit_class_->IsDerivedFrom(pcd);
			else
				return false;
		}

		__inline bool				IsAbstract()const{return abstract_;}
		__inline void				SetAbstract(bool b){abstract_ = b;}
	private:
		///father class
		ClassDesc*						inherit_class_;
		///class name
		string							str_class_name_;
		bool							abstract_;
	};
#endif // USE_RTTI


}

#ifdef USE_RTTI
#define ENGINE_CLASS_DEF \
public:\
	virtual tng::ClassDesc*		GetType()const;\
	virtual bool				IsType(const tng::ClassDesc* _type)const;\
	virtual bool				IsDerivedFrom(const tng::ClassDesc* _type)const;\
	static void					InitStatic();\
	static tng::ClassDesc*		GetRtti(){static tng::ClassDesc staticRTTI;return &staticRTTI;}\
	static tng::PropertySet*	GetPropertySet(){static tng::PropertySet staticPropertySet;return &staticPropertySet;}

#define START_ABSTRACT_CLASS(class_name)\
	tng::ClassDesc*	class_name::GetType()const{return class_name::GetRtti();}\
	bool		class_name::IsType(const tng::ClassDesc* _type)const{return GetType()==_type;}\
	bool		class_name::IsDerivedFrom(const tng::ClassDesc* _type)const{return class_name::GetRtti()->IsDerivedFrom(_type);}\
	void class_name::InitStatic(){\
	class_name::GetRtti()->SetClassName(string(#class_name));\
	class_name::GetRtti()->SetAbstract(true);\
	tng::Object::GetAllClasses().push_back(class_name::GetRtti());

#define START_CLASS(class_name)\
	START_ABSTRACT_CLASS(class_name)\
	tng::Object::dynamic_factory_static.RegisterClass<class_name>(#class_name);\
	class_name::GetRtti()->SetAbstract(false);

#define START_SUB_CLASS(class_name,parents)\
	START_CLASS(class_name)\
	class_name::GetRtti()->SetInheritClass(parents::GetRtti());

#define START_SUB_ABSTRACT_CLASS(class_name,parents)\
	START_ABSTRACT_CLASS(class_name)\
	class_name::GetRtti()->SetInheritClass(parents::GetRtti());
#else
#define ENGINE_CLASS_DEF \
public:\
	static void					InitStatic(); \
	static tng::PropertySet*	GetPropertySet(){ static tng::PropertySet staticPropertySet; return &staticPropertySet; }

#define START_ABSTRACT_CLASS(class_name)\
	void class_name::InitStatic(){

#define START_CLASS(class_name)\
	START_ABSTRACT_CLASS(class_name)\
	tng::Object::GetDynamicFactory().RegisterClass<class_name>(#class_name);

#define START_SUB_CLASS(class_name,parents)\
	START_CLASS(class_name)

#define START_SUB_ABSTRACT_CLASS(class_name,parents)\
	START_ABSTRACT_CLASS(class_name)
#endif//USE_RTTI
#define END_CLASS }
#define Property(class_name,property_type,name,g,s)\
	GetPropertySet()->AddProperty(name, AccesserPtr(new Accesser<class_name, property_type>(&class_name::g, &class_name::s)));
