#include "resource/resource_manger.h"
#include "engine/engine.h"
#include "core/encode.h"
namespace tng
{
	ResourceManger*			ResourceManger::manger_ =NULL;
#ifndef TNG_ANDROID
#define ENGINE_ASSET  string("../EngineAsset/")
	string				ResourceManger::asset_path_ = "";
#endif
	ResourceManger* ResourceManger::CreateManger()
	{
		manger_ = Engine::GetInstance()->CreateService("ResourceManger","ResourceManger","DefaultLoop")->DynamicCast<ResourceManger>();
		Log::GetLog()->Printf(Log::SYS_CHN, "ResourceManger created");
		static LZ4Coder c;
		Coder::SetDefaultCoder(&c);
		return manger_;
	}
	void ResourceManger::DestroyManger()
	{
		manger_->RemoveAllResource();
		//Engine::GetInstance()->DestoryService("ResourceManger");
	}
	ResourceManger::ResourceManger(){
		loader_factory_.RegisterClass<LocalFileStreamLoader>(LocalFilesLoader);
	}
	ResourceManger::~ResourceManger(){
		RemoveAllResource();
		for (DecoderMap::iterator it = decoder_map_.begin();
			it!=decoder_map_.end();
			it++)
		{
			delete it->second;
		}
		decoder_map_.clear();
	}
	StreamLoader* ResourceManger::AddLoader(u32 loaderid)
	{
		StreamLoader* loader = loader_factory_.CreateObject(loaderid);
		if (loader)
		{
			loader_map_[loaderid]=loader;
		}
// #ifndef TNG_ANDROID
// 		if(asset_path_.size())
// 			loader->SetAssetPath(asset_path_);
// #else
// 		loader->SetAssetPath("");
// #endif
			
		return loader;
	}
	void ResourceManger::SetAssetPath(const string& path)
	{
// 		for (LoaderMap::iterator it = loader_map_.begin();
// 			it!=loader_map_.end();
// 			it++)
// 		{
// 			it->second->SetAssetPath(path);
// 		}
#ifndef TNG_ANDROID
		asset_path_ =path;
#endif
	}
	std::istream* ResourceManger::LoadAssetStream(const string& key)
	{
		for (LoaderMap::iterator it = loader_map_.begin();
			it!=loader_map_.end();
			it++)
		{	
			std::istream* stream = it->second->Load(key);
			if (stream)
			{
				return stream;
			}
		}
		return NULL;
	}

	string ResourceManger::GetAssetPath(const string& path)
	{
		string final_path;

#ifdef TNG_ANDROID
		s32 pos = (s32)path.find("asset:/");
		if (pos!=-1)
		{
			pos += (s32)strlen("asset:/");
			final_path = string("GameAsset/")+path.substr(pos);
		}
		else
		{
			pos = (s32)path.find("engine:/");
			if (pos!=-1)
			{
				pos += (s32)strlen("engine:/");
				final_path = string("EngineAsset/")+path.substr(pos);
			}
			else
				final_path = path;
		}
#else
		s32 pos = (s32)path.find("asset:/");
		if (pos!=-1)
		{
			pos += (s32)strlen("asset:/");
			final_path = asset_path_+path.substr(pos);
		}
		else
		{
			pos = (s32)path.find("engine:/");
			if (pos!=-1)
			{
				pos += (s32)strlen("engine:/");
				final_path = ENGINE_ASSET+path.substr(pos);
			}
			else
				final_path = path;
		}
#endif // TNG_ANDROID

		return final_path;
	}

	void ResourceManger::FindFilesInAssetPath(const string& path,vector<Path>& files)
	{
		//string final_path = ;
#ifdef TNG_ANDROID
		FileSystem::FindFilesInAPK(GetAssetPath(path),files);
#else
		FileSystem::FindFiles(GetAssetPath(path),files);
#endif // TNG_ANDROID

		
	}
	ResDecoder* ResourceManger::GetDecoder(const string& name)
	{
		string lowerstr = name;
		ToLower(lowerstr);
		DecoderMap::iterator it = decoder_map_.find(lowerstr);
		if (it!=decoder_map_.end())
		{
			return it->second;
		}
		return NULL;
	}
	bool ResourceManger::_create(ResPtr ptr,const string& key)
	{
		for (LoaderMap::iterator it = loader_map_.begin();
			it!=loader_map_.end();
			it++)
		{	
			std::istream* stream = it->second->Load(key);
			if (stream)
			{
				ptr->Create(*stream);
				return true;
			}
			else
			{
				return false;
			}
			
		}
		return false;
	}
	void ResourceManger::_update(f32 deltaTime)
	{
		UpdateableService::_update(deltaTime);
		loading_thread_.UpdateDoneList();
	}

	START_SUB_CLASS(ResourceManger,UpdateableService)
	END_CLASS
}