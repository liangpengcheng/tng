#include "modelimport/texture_imp.h"
#include "resil/include/IL/il.h"
//#include "resil/include/IL/ilut.h"
#include "resil/include/IL/devil_cpp_wrapper.hpp"
#include "core/binary_reader_writer.h"
#include "graphics/texture.h"
#include "core/encode.h"
namespace tng
{

	bool TextureDecoder::Decode(std::istream&stream_in,JsonObject* config,std::ostream&stream_out)
	{
		stream_in.seekg (0, stream_in.end);
		int length = (int)stream_in.tellg();
		stream_in.seekg (0, stream_in.beg);

		if (length>=0)
		{
			BinaryReader reader(stream_in);
			char* buf = new char[length];
			reader.readRaw(buf,length);

			ilImage img;
			if(!img.Load(buf,length,ext_))
			{
				delete []buf;
				return false;
			}
			TextureData td;
			if(config)
				td.LoadConfig(config);
			td.head_.width_  = img.Width();
			td.head_.height_= img.Height();
			td.head_.faces_ = img.NumImages();
			td.head_.mipmaps_ = img.NumMipmaps();
			td.head_.bpp_ = img.Bpp();
			td.head_.type_ = Tex_2D;

			if (ext_== IL_TGA)
			{
				if (img.Format()==IL_RGB)
				{
					td.head_.fmt_ = PixelFormat::R8G8B8;
				}
				else if (img.Format()==IL_BGR)
				{
					img.Convert(IL_RGB);
					td.head_.fmt_ = PixelFormat::R8G8B8;
				}
				else if (img.Format()==IL_RGBA)
				{
					td.head_.fmt_ = PixelFormat::A8R8G8B8;
				}
				else if (img.Format()==IL_BGRA)
				{
					img.Convert(IL_RGBA);
					td.head_.fmt_ = PixelFormat::A8R8G8B8;
				}
				else if (img.Format()==IL_LUMINANCE)
				{
					td.head_.fmt_ = PixelFormat::L8;
				}
				else if (img.Format()==IL_LUMINANCE_ALPHA)
				{
					td.head_.fmt_ = PixelFormat::L8A8;
				}
				else if (img.Format()==IL_ALPHA)
				{
					td.head_.fmt_ = PixelFormat::A8;
				}
			}
			else if (ext_==IL_HDR)
			{
				td.head_.fmt_ = PixelFormat::A16B16G16R16F;
			}
			else
			{
				Log::GetLog()->Printf(Log::ERROR_CHN,"does not support this format yet:%d",img.Format());
				delete []buf;
				return false;
			}

			if (!IsPow2(td.head_.width_)||!IsPow2(td.head_.height_))
			{
				Log::GetLog()->Printf(Log::ERROR_CHN,"width or height is not power of 2");
				delete []buf;
				return false;
			}

			td.raw_ = new char[td.size()];
	
			memcpy(td.raw_,img.GetData(),td.size());
			
			td.Save(stream_out);

			delete []buf;
		}
		return true;
	}
	void TextureDecoder::Init()
	{
		ResourceManger::GetInstance()->AddDecoder("tga",new TextureDecoder(IL_TGA));
		ResourceManger::GetInstance()->AddDecoder("hdr",new TextureDecoder(IL_HDR));
	}
	
	void TextureDecoder::Exit()
	{
		ilShutDown();
	}
}