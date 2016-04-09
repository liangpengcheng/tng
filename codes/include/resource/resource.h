#ifndef _TNG_RESOURCE_H_
#define _TNG_RESOURCE_H_
#include "resource/resource_macro.h"
#include "core/core_macros.h"
#include "core/refobject.h"
#include "core/autoptr.h"
#include "core/threadfun.h"
#include "core/delegate.h"
#include <string>
namespace tng
{
	class RES_API ResourceBase:public RefCountedObject
	{
	public:
		ResourceBase():loaded_(false){}
		virtual ~ResourceBase(){}
		const std::string& Key()const{return key_;}
		void SetKey(const std::string& k){key_=k;}
		virtual void Create(std::istream& stream)=0;
		virtual void CreateFinished(){loaded_ = true;}
		bool Loaded()const{return loaded_;}
	protected:
		std::string		key_;
		bool			loaded_;
	};

	typedef AutoPtr<ResourceBase> ResPtr;

	class RES_API Resource :public ResourceBase
	{
	public:
		Resource(){}
		~Resource(){}
		
		void Create(std::istream& stream)
		{
			Load(stream);
		}
		virtual void CreateFinished(){
			ResourceBase::CreateFinished();
			if (onloaded_)
			{
				onloaded_();
			}
		}
		typedef Delegate0<>	OnLoaded;
		void SetOnloaded(OnLoaded l){
			onloaded_ = l;
		}
		void ClearOnloaded(){
			onloaded_ = NULL;
		}
	protected:
		virtual void Load(std::istream& sream) = 0;
		virtual void Save(std::ostream& stream) = 0;
		OnLoaded			onloaded_;
	};
}

#endif