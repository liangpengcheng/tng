#include "core/filesystem.h"
#include "resource/stream_loader.h"
#include "resource/resource_manger.h"
#include "core/stl_rebind.h"
#include "core/encode.h"
namespace tng
{

	std::istream* LocalFileStreamLoader::Load(const string& key)
	{
		string final_path = ResourceManger::GetAssetPath(key);
		
		Path path(key);
		string ext;
		string dir;
		string fn;
		path.Split(dir,fn,ext);
		ResDecoder* dec = ResourceManger::GetInstance()->GetDecoder(ext);
		if (dec)
		{
#ifdef TNG_ANDROID
			FileInputStreamAPK file(final_path);
#else
			FileInputStream file(final_path);
#endif // TNG_ANDROID

				
			std::iostream* stream = new stringstream(std::ios_base::binary|std::ios_base::in | std::ios_base::out);
			JsonDocument config;
			Path path;
			path.InitPath(final_path);
			string ext,fn,dir;
			path.Split(dir,fn,ext);
			string cfgpath = dir+fn+".res";
// fix it
			if (
#ifdef TNG_OS_FAMILY_WINDOWS
				FileSystem::IsFileExist(cfgpath)
#else
				0
#endif // TNG_OS_FAMILY_WINDOWS
				)
			{
				FileInputStream cfg(cfgpath);
				string json;
				string line;
				while(std::getline(cfg,line))
				{
					json = json+line;
				}

				config.Parse<0>(json.c_str());
				dec->Decode(file,&config,*stream);
			}
			else
			{
				dec->Decode(file,NULL,*stream);
			}
			
			stream->seekp(0);
			return stream;
		}
		else
		{
#ifdef TNG_ANDROID
			std::istream* stream = new FileInputStreamAPK (final_path);
#else
			std::istream* stream = new FileInputStream (final_path);
#endif // TNG_ANDROID
			return stream;
		}

	}

// 	void LocalFileStreamLoader::PreLoadAssetPath(const string& path)
// 	{
// 		std::vector<Path> paths;
// #ifdef TNG_ANDROID
// 		//FileSystem::FindFiles("",paths,FileSystem::FFile,1);
// #else
// 		FileSystem::FindFiles(path,paths,FileSystem::FFile,1);
// #endif
// 		for (s32 i = 0;i <paths.size();i++)
// 		{
// 			valid_resource_.insert(paths[i]);
// 
// #ifdef _DEBUG
// 			Log::GetLog()->Printf(Log::DBG_CHN,"find file %s",(string(paths[i])).c_str());
// #endif // DEBUG
// 		}


//	}

	bool LZ4Decoder::Decode(std::istream&stream_in, JsonObject* config, std::ostream&stream_out)
	{
		LZ4Coder coder;
		//fixit 这个地方多了一次内存分配，不是直接进入streamout的
		return coder.Decode(stream_in, stream_out);
	}
}