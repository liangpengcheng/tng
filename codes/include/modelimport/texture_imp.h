#ifndef _TNG_TEX_IMP_H_
#define _TNG_TEX_IMP_H_
#include "resource/resource_manger.h"
#include "model_imp_macros.h"

namespace tng
{
	class TextureDecoder :public ResDecoder
	{
	public:
		TextureDecoder(u32 ext):ext_(ext){};
		bool	Decode(std::istream&stream_in,JsonObject* config,std::ostream&stream_out)TNG_OVERRIDE;
		virtual string			ResType()const TNG_OVERRIDE{ return "texture"; }
		static void Init();
		static void Exit();
	protected:
		u32		ext_;
	};
}

#endif