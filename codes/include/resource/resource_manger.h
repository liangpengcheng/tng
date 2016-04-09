#ifndef _RESOURCE_MANGER_H_
#define _RESOURCE_MANGER_H_
#include "engine/service.h"
#include "resource.h"
#include "stream_loader.h"
#include <unordered_map>
#include "core/path.h"
#include "engine/thread_pool.h"

namespace tng
{
	
	class RES_API ResourceManger:public UpdateableService
	{
	public:
		ENGINE_CLASS_DEF
		class ResLoadReq:public Task
		{
		public:
			virtual void Request(){
				manger_->_create(ptr_,key_);
			}
			virtual void Response()
			{
				ptr_->CreateFinished();
			}
			ResPtr ptr_;
			string key_;
			ResourceManger* manger_;
		};

		
		ResourceManger();
		~ResourceManger();
		static ResourceManger* CreateManger();
		static void				DestroyManger();

		typedef unordered_map<string,ResPtr > ResourceMap;

		typedef unordered_map<u32,StreamLoader*>		LoaderMap;
		typedef unordered_map<string,ResDecoder*>	DecoderMap;
		
		template <typename T>
		AutoPtr<T>				Create(const string& key,bool async = true)
		{
			ResourceMap::iterator it = resource_map_.find(key);
			if (it!=resource_map_.end())
			{
				 
				return (T*)it->second.get();
			}
			else
			{
				AutoPtr<T> ptr = new T();
				ResLoadReq* req = new ResLoadReq;
				req->manger_=this;
				req->key_=key;
				req->ptr_=ptr;
				if(async)
					loading_thread_.AddTask(req);
				else
				{
					req->Request();
					req->Response();
					delete req;
				}
				resource_map_[key] = ResPtr(ptr);
				return ptr;
			}
		}
		__inline void RemoveAllResource()
		{
			resource_map_.clear();
		}
		void				SetLoadingThread(u32 num){loading_thread_.Initialize(num);}
		StreamLoader*		AddLoader(u32 loaderid);
		StreamLoader*		GetLoader(u32 loaderid){
			return loader_map_[loaderid];
		}
		LoaderFactory*		GetLoaderFactory(){return &loader_factory_;}
		/**
			set asset root directory
			@remark in android the root work directory is "" ,so param is not valid .
		*/
		static void				SetAssetPath(const string& path);
		std::istream*		LoadAssetStream(const string& key);

		static void			FindFilesInAssetPath(const string& path,vector<Path>& files);

		static string	GetAssetPath(const string& path);
		/**
			@param name must be a lower string
		*/
		void				AddDecoder(const string& name,ResDecoder* dec){decoder_map_[name]=dec;}
		ResDecoder*			GetDecoder(const string& name);
	
		bool 				_create(ResPtr ptr,const string& key);
		void				_update(f32 deltaTime);
		static ResourceManger* GetInstance(){return manger_;}

	protected:
		static ResourceManger*			manger_;
		DecoderMap						decoder_map_;
		ResourceMap						resource_map_;
		LoaderFactory					loader_factory_;
		LoaderMap						loader_map_;
		ThreadPool						loading_thread_;
#ifndef TNG_ANDROID
		static string				asset_path_;
#endif
		
	};
}
#endif