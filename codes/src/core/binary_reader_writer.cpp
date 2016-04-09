#include "core/binary_reader_writer.h"
#include "core/endian.h"
#include "core/arch_detect.h"

namespace tng
{
	BinaryWriter::BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder) :
		ostr_(ostr)
	{
#ifdef TNG_BIGENDIAN
		flipBytes_ = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#else
		flipBytes_ = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#endif
	}



	BinaryWriter::~BinaryWriter()
	{

	}


	BinaryWriter& BinaryWriter::operator << (bool value)
	{
		u8 uv = u8(value);
		ostr_.write((const char*)&uv, sizeof(uv));
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (u8 value)
	{
		ostr_.write((const char*)&value, sizeof(value));
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (s8 value)
	{
		ostr_.write((const char*)&value, sizeof(value));
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (s16 value)
	{
		if (flipBytes_)
		{
			Swap_Endian_Fun<s16> swap;
			s16 fValue;
			swap(value, &fValue);
			ostr_.write((const char*)&fValue, sizeof(fValue));
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (u16 value)
	{
		if (flipBytes_)
		{
			Swap_Endian_Fun<u16> swap;
			u16 fValue;
			swap(value, &fValue);
			ostr_.write((const char*)&fValue, sizeof(fValue));
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (s32 value)
	{
		if (flipBytes_)
		{
			Swap_Endian_Fun<s32> swap;
			s32 fValue;
			swap(value, &fValue);
			ostr_.write((const char*)&fValue, sizeof(fValue));
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (u32 value)
	{
		if (flipBytes_)
		{
			Swap_Endian_Fun<u32> swap;
			u32 fValue;
			swap(value, &fValue);
			ostr_.write((const char*)&fValue, sizeof(fValue));
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (f32 value)
	{
		if (flipBytes_)
		{
			const char* ptr = (const char*)&value;
			ptr += sizeof(value);
			for (unsigned i = 0; i < sizeof(value); ++i)
				ostr_.write(--ptr, 1);
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (f64 value)
	{
		if (flipBytes_)
		{
			const char* ptr = (const char*)&value;
			ptr += sizeof(value);
			for (unsigned i = 0; i < sizeof(value); ++i)
				ostr_.write(--ptr, 1);
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}



	BinaryWriter& BinaryWriter::operator << (s64 value)
	{
		if (flipBytes_)
		{
			Swap_Endian_Fun<s64> swap;
			s64 fValue;
			swap(value, &fValue);

			ostr_.write((const char*)&fValue, sizeof(fValue));
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (u64 value)
	{
		if (flipBytes_)
		{
			Swap_Endian_Fun<u64> swap;
			u64 fValue;
			swap(value, &fValue);

			ostr_.write((const char*)&fValue, sizeof(fValue));
		}
		else
		{
			ostr_.write((const char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryWriter& BinaryWriter::operator << (const string& value)
	{

		u32 length = (u32)value.size();
		(*this) << length;
		ostr_.write(value.data(), length);

		return *this;
	}
	BinaryWriter& BinaryWriter::operator << (const float2& value)
	{
		*this << value.x;
		*this << value.y;
		return *this;
	}
	BinaryWriter& BinaryWriter::operator << (const float3& value)
	{
		*this << value.x;
		*this << value.y;
		*this << value.z;
		return *this;
	}
	BinaryWriter& BinaryWriter::operator << (const float4& value)
	{
		*this << value.x;
		*this << value.y;
		*this << value.z;
		*this << value.w;
		return *this;
	}
	BinaryWriter& BinaryWriter::operator << (const float3x3& value)
	{
		*this << value.Row(0);
		*this << value.Row(1);
		*this << value.Row(2);
		return *this;
	}
	BinaryWriter& BinaryWriter::operator << (const float4x4& value)
	{
		*this << value.Row(0);
		*this << value.Row(1);
		*this << value.Row(2);
		*this << value.Row(3);
		return *this;
	}
	BinaryWriter& BinaryWriter::operator << (const Quat& value)
	{
		*this << value.x;
		*this << value.y;
		*this << value.z;
		*this << value.w;
		return *this;
	}
	void BinaryWriter::writeRaw(const string& rawData)
	{
		ostr_.write(rawData.data(), (std::streamsize) rawData.length());
	}


	void BinaryWriter::writeRaw(const char* buffer, std::streamsize length)
	{
		ostr_.write(buffer, length);
	}


	void BinaryWriter::flush()
	{
		ostr_.flush();
	}




	BinaryReader::BinaryReader(std::istream& istr, StreamByteOrder byteOrder) :
		istr_(istr)
	{
#if (TNG_BIGENDIAN)
		flipBytes_ = (byteOrder == BIG_ENDIAN_BYTE_ORDER);
#else
		flipBytes_ = (byteOrder == LITTLE_ENDIAN_BYTE_ORDER);
#endif
	}





	BinaryReader::~BinaryReader()
	{

	}


	BinaryReader& BinaryReader::operator >> (bool& value)
	{
		u8 uv;
		istr_.read((char*)&uv, sizeof(uv));
		value = uv == 0 ? false : true;
		return *this;
	}




	BinaryReader& BinaryReader::operator >> (u8& value)
	{
		istr_.read((char*)&value, sizeof(value));
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (s8& value)
	{
		istr_.read((char*)&value, sizeof(value));
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (s16& value)
	{
		istr_.read((char*)&value, sizeof(value));

		if (flipBytes_)
		{
			Swap_Endian_Fun<s16> swap;
			s16 fValue;
			swap(value, &fValue);
			value = fValue;
		}
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (u16& value)
	{
		istr_.read((char*)&value, sizeof(value));
		if (flipBytes_)
		{
			Swap_Endian_Fun<u16> swap;
			u16 fValue;
			swap(value, &fValue);
			value = fValue;
		}
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (s32& value)
	{
		istr_.read((char*)&value, sizeof(value));
		if (flipBytes_)
		{
			Swap_Endian_Fun<s32> swap;
			s32 fValue;
			swap(value, &fValue);
			value = fValue;
		}
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (u32& value)
	{
		istr_.read((char*)&value, sizeof(value));
		if (flipBytes_)
		{
			Swap_Endian_Fun<u32> swap;
			u32 fValue;
			swap(value, &fValue);
			value = fValue;
		}
		return *this;
	}



	BinaryReader& BinaryReader::operator >> (f32& value)
	{
		if (flipBytes_)
		{
			char* ptr = (char*)&value;
			ptr += sizeof(value);
			for (unsigned i = 0; i < sizeof(value); ++i)
				istr_.read(--ptr, 1);
		}
		else
		{
			istr_.read((char*)&value, sizeof(value));
		}
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (f64& value)
	{
		if (flipBytes_)
		{
			char* ptr = (char*)&value;
			ptr += sizeof(value);
			for (unsigned i = 0; i < sizeof(value); ++i)
				istr_.read(--ptr, 1);
		}
		else
		{
			istr_.read((char*)&value, sizeof(value));
		}
		return *this;
	}



	BinaryReader& BinaryReader::operator >> (s64& value)
	{
		istr_.read((char*)&value, sizeof(value));
		if (flipBytes_)
		{
			Swap_Endian_Fun<s64> swap;
			s64 fValue;
			swap(value, &fValue);
			value = fValue;
		}
		return *this;
	}


	BinaryReader& BinaryReader::operator >> (u64& value)
	{
		istr_.read((char*)&value, sizeof(value));
		if (flipBytes_)
		{
			Swap_Endian_Fun<u64> swap;
			u64 fValue;
			swap(value, &fValue);
			value = fValue;
		}
		return *this;
	}




	BinaryReader& BinaryReader::operator >> (string& value)
	{
		u32 size = 0;
		(*this) >> size;
		value.clear();
		if (!istr_.good()) return *this;
		value.reserve(size);
		while (size--)
		{
			char c;
			if (!istr_.read(&c, 1).good()) break;
			value += c;
		}

		return *this;
	}





	void BinaryReader::readRaw(std::streamsize length, string& value)
	{
		value.clear();
		value.reserve(static_cast<string::size_type>(length));
		while (length--)
		{
			char c;
			if (!istr_.read(&c, 1).good()) break;
			value += c;
		}
	}


	void BinaryReader::readRaw(char* buffer, std::streamsize length)
	{
		istr_.read(buffer, length);
	}

	BinaryReader& BinaryReader::operator >> (float2& value)
	{
		*this >> value.x;
		*this >> value.y;
		return *this;
	}
	BinaryReader& BinaryReader::operator >> (float3& value)
	{
		*this >> value.x;
		*this >> value.y;
		*this >> value.z;
		return *this;
	}
	BinaryReader& BinaryReader::operator >> (float4& value)
	{
		*this >> value.x;
		*this >> value.y;
		*this >> value.z;
		*this >> value.w;
		return *this;
	}
	BinaryReader& BinaryReader::operator >> (float3x3& value)
	{
		*this >> value.Row(0);
		*this >> value.Row(1);
		*this >> value.Row(2);
		return *this;
	}
	BinaryReader& BinaryReader::operator >> (float4x4& value)
	{
		*this >> value.Row(0);
		*this >> value.Row(1);
		*this >> value.Row(2);
		*this >> value.Row(3);
		return *this;
	}
	BinaryReader& BinaryReader::operator >> (Quat& value)
	{
		*this >> value.x;
		*this >> value.y;
		*this >> value.z;
		*this >> value.w;
		return *this;
	}



}