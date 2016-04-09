#include "core/encode.h"
#include "core/log.h"
#include "lz4/lib/lz4.h"
#include "lz4/lib/lz4hc.h"
#include "core/threadfun.h"
#define MAX_DECODE_SIZE (5*1024*1024)
namespace tng
{
	bool RLECompressor::Encode(std::istream& _in, std::ostream& _out)
	{
		switch (size_)
		{
		case 1:
			EncodeT<u8>(_in, _out);
			break;
		case 2:
			EncodeT<u16>(_in, _out);
			break;
		case 4:
			EncodeT<u32>(_in, _out);
			break;
		case 8:
			EncodeT<f64>(_in, _out);
			break;
		default:
			Log::GetLog()->Printf(Log::ERROR_CHN, "rle must be 1,2,4,8");
			break;
		}
		return true;

	}

	bool RLECompressor::Decode(std::istream& _in, std::ostream& _out)
	{
		switch (size_)
		{
		case 1:
			DecodeT<u8>(_in, _out);
			break;
		case 2:
			DecodeT<u16>(_in, _out);
			break;
		case 4:
			DecodeT<u32>(_in, _out);
			break;
		case 8:
			DecodeT<f64>(_in, _out);
			break;
		default:
			Log::GetLog()->Printf(Log::ERROR_CHN, "rle must be 1,2,4,8");
			break;
		}
		return true;
	}
	static char buf1[MAX_DECODE_SIZE];
	static char buf2[MAX_DECODE_SIZE];
	Mutex lz4Locker;
	bool LZ4Coder::Encode(std::istream& _in, std::ostream& _out)
	{

		_in.seekg(0, _in.end);
		size_t len = _in.tellg();
		_in.seekg(0, _in.beg);
		BinaryReader br(_in);

		if (MAX_DECODE_SIZE < len || MAX_DECODE_SIZE < LZ4_compressBound((int)len))
		{
			Log::GetLog()->Printf(Log::ERROR_CHN, "lz compress size is larger than 5mb");
			return false;
		}
		AutoLock<Mutex> ___l(&lz4Locker);
		char* buf = buf1;
		char* destbuf = buf2;
		br.readRaw((char*)buf, len);
		int destlen = LZ4_compressHC(buf, destbuf, (int)len);
		if (destlen <= 0){
			return false;
		}
		BinaryWriter bw(_out);
		bw << s32(len);
		bw.writeRaw(destbuf, destlen);
		return true;
	}

	bool LZ4Coder::Decode(std::istream& _in, std::ostream& _out)
	{

		_in.seekg(0, _in.end);
		size_t len = _in.tellg();
		_in.seekg(0, _in.beg);
		BinaryReader br(_in);

		s32 orglen;
		br >> orglen;

		if (MAX_DECODE_SIZE < len || MAX_DECODE_SIZE < orglen)
		{
			Log::GetLog()->Printf(Log::ERROR_CHN, "lz compress size is larger than 5mb");
			return false;
		}
		AutoLock<Mutex> ___l(&lz4Locker);
		char* destbuf = buf1;
		char* buf = buf2;
		br.readRaw((char*)buf, len - 4);

		int compressSz = LZ4_decompress_fast(buf, destbuf, orglen);
		if (compressSz == len - 4)
		{
			BinaryWriter bw(_out);
			bw.writeRaw(destbuf, orglen);
			return true;
		}
		else
		{
			return false;
		}
	}
	Coder* Coder::defaultCoder = NULL;
}