#include "graphics/texture.h"
#include "core/binary_reader_writer.h"
#include "resource/resource_manger.h"
#include "graphics/graphics.h"
#include "graphics/renderservice.h"
#include "core/encode.h"
namespace tng
{
	void TextureResource::Load(std::istream& stream)
	{
		if (data_.isNull()){
			data_ = TextureDataPtr(new TextureData());
		}
		data_->Load(stream);
	}
	void TextureResource::Save(std::ostream& stream)
	{
		data_->Save(stream);
	}
	TextureData::TextureData():raw_(NULL)
#ifndef NO_RENDER_THREAD
		,using_(false)
#endif
		,render_tex_(0),auto_mipmap_(false),onebit_alpha_(true){}
	TextureData::~TextureData(){
#ifndef NO_RENDER_THREAD
		if(using_)
			Log::GetLog()->Printf(Log::ERROR_CHN,"still using in render thread ,delete it will raise exception");
#endif
		SAFE_DELETE_ARRAY(raw_);
	}
	void TextureData::Load(std::istream& stream)
	{
#ifndef NO_RENDER_THREAD
		using_ = true;
#endif
		std::iostream* cstream = new stringstream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
		Coder &coder = *Coder::GetDefaultCoder();
		coder.Decode(stream, *cstream);
		cstream->seekp(0);
		BinaryReader br(*cstream);
		u32 version;
		br>>version;
		//header
		u32 type;
		u32 filter_min, filter_max, wrap_s, wrap_t;
		br >> head_.height_ >> head_.width_ >> head_.faces_ >> head_.mipmaps_ >> head_.bpp_ >> head_.fmt_ >> type >> auto_mipmap_ >> onebit_alpha_ >> filter_min >> filter_max>>wrap_s>>wrap_t;
		head_.state_.min_ = TextureFilter(filter_min);
		head_.state_.mag_ = TextureFilter(filter_max);
		head_.state_.s_ = WrapMode(wrap_s);
		head_.state_.t_ = WrapMode(wrap_t);
		head_.type_ = TextureType(type);
		u32 current_size = size();
		if (current_size>0)
		{
			raw_ = new char[current_size];
			br.readRaw(raw_,current_size);
		}
		
		render_tex_ = internal::GetRenderService()->CreateTexture(this);
#ifndef NO_RENDER_THREAD
		using_ = false;
#endif
		delete cstream;
	}

	void TextureData::Save(std::ostream& stream)
	{
		std::iostream* cstream = new stringstream(std::ios_base::binary | std::ios_base::in | std::ios_base::out);
		BinaryWriter bw(*cstream);
		bw<<u32(TEXTURE_FORMAT_VERSION);
		//header
		bw << head_.height_ << head_.width_ << head_.faces_ << head_.mipmaps_ << head_.bpp_ << head_.fmt_ << u32(head_.type_) << 
			auto_mipmap_ << onebit_alpha_ << 
			(u32)head_.state_.min_ << (u32)head_.state_.mag_ << (u32)head_.state_.s_ << (u32)head_.state_.t_;
		u32 current_size = size();
		Assert(raw_);
		if (current_size>0)
		{
			bw.writeRaw(raw_,current_size);
		}
		cstream->seekp(0);
		Coder &coder = *Coder::GetDefaultCoder();
		coder.Encode(*cstream, stream);
		delete cstream;
	}
	TextureResource::~TextureResource()
	{
		if(GetTexture())
			internal::GetRenderService()->DeleteTexture(GetTexture());
	}
	AutoPtr<FileTexture> FileTexture::LoadResource(const string& key, bool ansync /*= true*/)
	{
		AutoPtr<FileTexture> r = ResourceManger::GetInstance()->Create<FileTexture>(key, ansync);
		r->SetOnloaded(Resource::OnLoaded(r.get(), &FileTexture::OnLoaded));
		return r;
	}
	void FileTexture::OnLoaded()
	{
		TextureData* data = resource_->GetData();
		if (data&&
			!data->head_.mipmaps_
			&&!data->auto_mipmap_)
		{
			head_ = data->head_;
			//has no mipmap
			//check filter fun
			if (head_.state_.min_>TF_Linear)
			{
				if (head_.state_.min_>TF_PointMipLiner)
				{
					head_.state_.min_ = TF_Linear;
				}
				else
					head_.state_.min_ = TF_Point;
			}
			if (head_.state_.mag_ > TF_Linear)
			{
				if (head_.state_.mag_ > TF_PointMipLiner)
				{
					head_.state_.mag_ = TF_Linear;
				}
				else
					head_.state_.mag_ = TF_Point;
			}
		}
	}
}