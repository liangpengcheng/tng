#ifndef _TNG_ATLAS_H_
#define _TNG_ATLAS_H_
#include "core/stl_rebind.h"
#include "core/refobject.h"
#include "core/math.h"
#include "graphics/texture.h"
#include "graphics/graphics_macros.h"
namespace tng
{
	class GFX_API Sprite :public RefCountedObject{
	public:
		float2			u0v0_;		//0-1
		float2			u1v1_;		//0-1
		TexturePtr		texture_;
		enum Type{
			SP_Simple,
			SP_Slice,
		} type_;
	};
	typedef AutoPtr<Sprite> SpritePtr;
	class GFX_API Atlas
	{
	public:
		bool Load(const string& jsonfile);

		__inline SpritePtr FindSprite(const string& spr){
			unordered_map<string, SpritePtr>::iterator it = sprite_map_.find(spr);
			if (it != sprite_map_.end())
				return it->second;
			else
				return NULL;
		}
	protected:
		unordered_map<string, SpritePtr>	sprite_map_;
	};
}
#endif