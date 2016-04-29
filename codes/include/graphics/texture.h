#ifndef _TNG_TEXTURE_H_
#define _TNG_TEXTURE_H_
#include "core/threadfun.h"
#include "core/math.h"
#include "graphics/renderbuffer.h"
#include "core/refobject.h"
#include "core/autoptr.h"
#include "graphics/graphics_macros.h"
#include "graphics/pixelformat.h"
#include "resource/resource.h"
#include "graphics/renderstate.h"
namespace tng
{
	enum TextureType
	{
		Tex_1D,
		Tex_2D,
		Tex_3D,
		Tex_Cube,
	};
	struct sampler_state
	{
		sampler_state() :min_(TF_LinerMipLiner), mag_(TF_LinerMipLiner), s_(WM_Clamp), t_(WM_Clamp){}
		TextureFilter min_;
		TextureFilter mag_;
		WrapMode	s_;
		WrapMode	t_;
	};

	struct texture_head
	{
		u32 width_, height_, faces_, mipmaps_, bpp_, fmt_;
		TextureType type_;
		sampler_state	state_;
	};

	class GFX_API TextureBase:public Resource
	{
	public:
		virtual ~TextureBase(){};
		virtual u32					GetTexture()const=0;
		texture_head				head_;
	};
	typedef AutoPtr<TextureBase>	TexturePtr;
	class GFX_API TextureData :public RefCountedObject
	{
	public:
		TextureData();
		~TextureData();
		
		u32 size(){
			u32 current_size = PixelFormat::GetMemorySize(head_.width_,head_.height_,Max<u32>(head_.faces_,1),PixelFormat::Code(head_.fmt_));
			u32 ret_size= current_size;
			for (u32 i = 0; i < head_.mipmaps_; i++)
			{
				current_size=PixelFormat::GetMemorySize(head_.width_>>(i+1),head_.height_>>(i+1),Max<u32>(head_.faces_,1),PixelFormat::Code(head_.fmt_));
				ret_size+=current_size;
			}
			//FIXIT read state_
			return ret_size;
		}
		texture_head head_;
		bool auto_mipmap_;
		bool onebit_alpha_;
		char* raw_;
#ifndef NO_RENDER_THREAD
		AtomicCounter		using_;
#endif
		
		void				Load(std::istream& stream);
		void				Save(std::ostream& stream);
		u32					GetTexture()const{ return render_tex_; }
		void				SetTexture(u32 tex){render_tex_=tex;}
		void				LoadConfig(JsonObject* cfg);
	protected:
		u32					render_tex_;
	};
	typedef AutoPtr<TextureData>	TextureDataPtr;
	FORCEINLINE void TextureData::LoadConfig(JsonObject* cfg)
	{
		if(cfg)
		{
			if((*cfg)["auto_mipmap"].IsBool())
				auto_mipmap_  = (*cfg)["auto_mipmap"].GetBool();
			if((*cfg)["onebit_alpha"].IsBool())
				onebit_alpha_ = (*cfg)["onebit_alpha"].GetBool();
			if ((*cfg)["filter_min"].IsUint())
				head_.state_.min_ = TextureFilter((*cfg)["filter_min"].GetUint());
			if ((*cfg)["filter_max"].IsUint())
				head_.state_.mag_ = TextureFilter((*cfg)["filter_max"].GetUint());
			if ((*cfg)["wrap_s"].IsUint())
				head_.state_.s_ = WrapMode((*cfg)["wrap_s"].GetUint());
			if ((*cfg)["wrap_t"].IsUint())
				head_.state_.t_ = WrapMode((*cfg)["wrap_t"].GetUint());
		}
	}
	
	class GFX_API TextureResource:public RefCountedObject
	{
	public:
		~TextureResource();
		void				Load(std::istream& stream);
		void				Save(std::ostream& stream);
		TextureDataPtr		GetData(){return data_;}
		u32					GetTexture()const{
			return data_->GetTexture();
		}
	protected:
		
		TextureDataPtr			data_;
	};

	class GFX_API FileTexture:public TextureBase
	{
	public:
		static AutoPtr<FileTexture> LoadResource(const string& key,bool ansync = true);
		u32					GetTexture()const{
			if (Loaded())
			{
				return resource_->GetTexture();
			}
			else
				return 0;
			
		}
		
	public:
		void				Load(std::istream& stream) TNG_OVERRIDE
		{
			if (!resource_)
			resource_ = new TextureResource;
			resource_->Load(stream);
		}
		void				Save(std::ostream& stream)TNG_OVERRIDE
		{
			if (resource_)
				resource_->Save(stream);
		}
	protected:
		void				OnLoaded();
		AutoPtr<TextureResource >		resource_;
	};
}

#endif