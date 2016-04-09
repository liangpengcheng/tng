#ifndef _TNG_PACKED_STREAM_LOADER_H_
#define _TNG_PACKED_STREAM_LOADER_H_

#include "stream_loader.h"
namespace tng
{
	class PackedStreamLoader:public StreamLoader
	{
	public:
		PackedStreamLoader(){
			Log::GetLog()->Printf(Log::SYS_CHN, "PackedStreamLoader added");
		}

		std::istream*	Load(const string& key);
		u32				GetPrimaryKey() { return PackedFileLoader; }
	};
}
#endif