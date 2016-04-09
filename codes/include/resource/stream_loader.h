#ifndef _TNG_STREAM_LOADER_H_
#define _TNG_STREAM_LOADER_H_
#include "core/dynamic_factory.h"
#include "core/filestream.h"

#include <vector>
#include <set>
#include "resource/resource_macro.h"

namespace tng
{
	enum LoaderType
	{
		LocalFilesLoader =0,
		PackedFileLoader =1,
		NetFileLoader	 =2,
	};

	class RES_API ResDecoder
	{
	public:
		virtual ~ResDecoder(){}
		virtual bool	Decode(std::istream&stream_in,JsonObject* config,std::ostream&stream_out) = 0;
		virtual string			ResType()const = 0;
	};

	class LZ4Decoder:public ResDecoder
	{
	public:
		bool	Decode(std::istream&stream_in, JsonObject* config, std::ostream&stream_out)TNG_OVERRIDE;
	};

	class RES_API StreamLoader
	{
	public:
		bool			IsValidResource(const string& key)const
		{
			return true;
		}
		
		
		virtual std::istream*	Load(const string& key) = 0;

		virtual u32				GetPrimaryKey()=0;


	protected:


	};
	/**
		load resource in single file
	*/
	class RES_API LocalFileStreamLoader:public StreamLoader
	{
	public:
		LocalFileStreamLoader(){
			Log::GetLog()->Printf(Log::SYS_CHN, "LocalFileStreamLoader added");
		}
		std::istream*	Load(const string& key);
		u32				GetPrimaryKey() {return LocalFilesLoader;}

	};



	class LoaderFactory:public DynamicFactory<StreamLoader,u32>
	{
	public:
	};
}

#endif