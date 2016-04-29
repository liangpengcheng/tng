#ifndef __NANOVG_PLUGIN_H__
#define __NANOVG_PLUGIN_H__
#include "../../include/graphics/renderservice.h"
#include "../../Include/core/log.h"
#include "nvg/nanovg.h"
namespace tng{
	struct VGContex{
		RenderService*		rservice_;

	};
	static __inline  int vgRenderCreate(void* uptr)
	{
		struct VGContex* vg = (struct VGContex*)uptr;
		Log::GetLog()->Printf(Log::SYS_CHN, "vg contex created");
		return 1;
	}

	static __inline int vgCreateTexture(void* uptr, int type, int w, int h, const unsigned char* data)
	{
		struct VGContex* vg = (struct VGContex*)uptr;
		TextureData *td= new TextureData ;
		td->auto_mipmap_ = true;
		td->using_ = true;
		td->raw_ = (char*)(data);
		
		return vg->rservice_->CreateTexture(td);
	}

	static __inline NVGcontext*	CreateContex(RenderService* rs){
		struct NVGparams params;
		struct NVGcontext* ctx = NULL;
		struct VGContex* contex = new VGContex();
		contex->rservice_ = rs;
		params.userPtr = contex;
		params.renderCreate = vgRenderCreate;
	}
}
#endif