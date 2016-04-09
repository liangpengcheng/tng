#ifndef _TNG_STREAM_RES_H_
#define _TNG_STREAM_RES_H_
#include "resource/resource.h"
#include <vector>
#include "resource/resource_manger.h"
#include "core/binary_reader_writer.h"
#include "core/smartptr.h"
namespace tng
{
	class AsyncStream:public Resource
	{
	public:
		AsyncStream(){}
		
		BinaryReader&		Get(){
			return *br_;
		}
	protected:
		void				Load(std::istream& stream) TNG_OVERRIDE{
			br_ = SmartPtr<BinaryReader>(new BinaryReader(stream));
		}
		void				Save(std::ostream& stream) TNG_OVERRIDE{}
		SmartPtr<BinaryReader>		br_;
	};
}

#endif