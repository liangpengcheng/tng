#ifndef _TNG_MODEL_IMP_H_
#define _TNG_MODEL_IMP_H_
#include "resource/resource_manger.h"
#include "model_imp_macros.h"
namespace tng
{
	class ModelDecoder :public ResDecoder
	{
	public:
		ModelDecoder(const char* ext):ext_(ext){}
		bool	Decode(std::istream&stream_in, JsonObject* config, std::ostream&stream_out)TNG_OVERRIDE;
		string	ResType()const TNG_OVERRIDE{ return "model"; }
		const char* GetExt()const{return ext_;}
		const char* ext_;
	};
}

#endif