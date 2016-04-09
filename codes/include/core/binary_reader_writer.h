#ifndef _BINARY_READER_WRITER_H
#define _BINARY_READER_WRITER_H
#include <vector>
#include <ostream>
#include <istream>
#include "core_macros.h"
#include "types.h"
#include "core/math.h"
#include "core/stl_rebind.h"
namespace tng
{

	class CORE_API BinaryWriter
	{
	public:
		enum StreamByteOrder
		{
			NATIVE_BYTE_ORDER = 1, /// the host's native byte-order
			BIG_ENDIAN_BYTE_ORDER = 2, /// big-endian (network) byte-order
			NETWORK_BYTE_ORDER = 2, /// big-endian (network) byte-order
			LITTLE_ENDIAN_BYTE_ORDER = 3  /// little-endian byte-order
		};

		BinaryWriter(std::ostream& ostr, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
		/// Creates the BinaryWriter.

		~BinaryWriter();
		/// Destroys the BinaryWriter.

		BinaryWriter& operator << (bool value);
		BinaryWriter& operator << (u8 value);
		BinaryWriter& operator << (s8 value);
		BinaryWriter& operator << (s16 value);
		BinaryWriter& operator << (u16 value);
		BinaryWriter& operator << (s32 value);
		BinaryWriter& operator << (u32 value);
		BinaryWriter& operator << (f32 value);
		BinaryWriter& operator << (f64 value);

		BinaryWriter& operator << (s64 value);
		BinaryWriter& operator << (u64 value);
		BinaryWriter& operator << (const float2& value);
		BinaryWriter& operator << (const float3& value);
		BinaryWriter& operator << (const float4& value);
		BinaryWriter& operator << (const float3x3& value);
		BinaryWriter& operator << (const float4x4& value);
		BinaryWriter& operator << (const Quat& value);

		BinaryWriter& operator << (const string& value);


		template <typename T>
		BinaryWriter& operator << (const vector<T>& value)
		{
			u32 size(static_cast<u32>(value.size()));

			*this << size;
			for (typename vector<T>::const_iterator it = value.begin(); it != value.end(); ++it)
			{
				*this << *it;
			}

			return *this;
		}


		void writeRaw(const string& rawData);
		/// Writes the string as-is to the stream.

		void writeRaw(const char* buffer, std::streamsize length);
		/// Writes length raw bytes from the given buffer to the stream.

		void flush();
		/// Flushes the underlying stream.

		bool good();
		/// Returns _ostr.good();

		bool fail();
		/// Returns _ostr.fail();

		bool bad();
		/// Returns _ostr.bad();

		std::ostream& stream() const;
		/// Returns the underlying stream.

		StreamByteOrder byteOrder() const;
		/// Returns the byte ordering used by the writer, which is
		/// either BIG_ENDIAN_BYTE_ORDER or LITTLE_ENDIAN_BYTE_ORDER.

	private:
		std::ostream&  ostr_;
		bool           flipBytes_;
	};

	inline std::ostream& BinaryWriter::stream() const
	{
		return ostr_;
	}


	inline bool BinaryWriter::good()
	{
		return ostr_.good();
	}


	inline bool BinaryWriter::fail()
	{
		return ostr_.fail();
	}


	inline bool BinaryWriter::bad()
	{
		return ostr_.bad();
	}
	inline BinaryWriter::StreamByteOrder BinaryWriter::byteOrder() const
	{
#if defined(TNG_BIGENDIAN)
		return flipBytes_ ? LITTLE_ENDIAN_BYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
#else
		return flipBytes_ ? BIG_ENDIAN_BYTE_ORDER : LITTLE_ENDIAN_BYTE_ORDER;
#endif
	}


	class CORE_API BinaryReader

	{
	public:
		enum StreamByteOrder
		{
			NATIVE_BYTE_ORDER = 1,  /// the host's native byte-order
			BIG_ENDIAN_BYTE_ORDER = 2,  /// big-endian (network) byte-order
			NETWORK_BYTE_ORDER = 2,  /// big-endian (network) byte-order
			LITTLE_ENDIAN_BYTE_ORDER = 3,  /// little-endian byte-order
			UNSPECIFIED_BYTE_ORDER = 4   /// unknown, byte-order will be determined by reading the byte-order mark
		};

		BinaryReader(std::istream& istr, StreamByteOrder byteOrder = NATIVE_BYTE_ORDER);
		/// Creates the BinaryReader.


		virtual~BinaryReader();
		/// Destroys the BinaryReader.

		BinaryReader& operator >> (bool& value);
		BinaryReader& operator >> (u8& value);
		BinaryReader& operator >> (s8& value);
		BinaryReader& operator >> (s16& value);
		BinaryReader& operator >> (u16& value);
		BinaryReader& operator >> (s32& value);
		BinaryReader& operator >> (u32& value);
		BinaryReader& operator >> (f32& value);
		BinaryReader& operator >> (f64& value);

		BinaryReader& operator >> (s64& value);
		BinaryReader& operator >> (u64& value);
		BinaryReader& operator >> (float2& value);
		BinaryReader& operator >> (float3& value);;
		BinaryReader& operator >> (float4& value);
		BinaryReader& operator >> (float3x3& value);
		BinaryReader& operator >> (float4x4& value);
		BinaryReader& operator >> (Quat& value);

		BinaryReader& operator >> (string& value);

		template <typename T>
		BinaryReader& operator >> (vector<T>& value)
		{
			u32 size(0);
			T elem;

			*this >> size;
			if (!good()) return *this;
			value.reserve(size);
			while (this->good() && size-- > 0)
			{
				*this >> elem;
				value.push_back(elem);
			}
			return *this;
		}

		void readRaw(std::streamsize length, string& value);
		/// Reads length bytes of raw data into value.

		void readRaw(char* buffer, std::streamsize length);
		/// Reads length bytes of raw data into buffer.

		bool good();
		/// Returns istr_.good();

		bool fail();
		/// Returns istr_.fail();

		bool bad();
		/// Returns istr_.bad();

		bool eof();
		/// Returns istr_.eof();

		std::istream& stream() const;
		/// Returns the underlying stream.

		StreamByteOrder byteOrder() const;
		/// Returns the byte-order used by the reader, which is
		/// either BIG_ENDIAN_BYTE_ORDER or LITTLE_ENDIAN_BYTE_ORDER.

		void setExceptions(std::ios_base::iostate st = (std::istream::failbit | std::istream::badbit));
		/// Sets the stream to throw exception on specified state (default failbit and badbit);

		std::streamsize available() const;
		/// Returns the number of available bytes in the stream.

	private:
		std::istream&  istr_;
		bool           flipBytes_;
	};




	inline bool BinaryReader::good()
	{
		return istr_.good();
	}


	inline bool BinaryReader::fail()
	{
		return istr_.fail();
	}


	inline bool BinaryReader::bad()
	{
		return istr_.bad();
	}


	inline bool BinaryReader::eof()
	{
		return istr_.eof();
	}


	inline std::istream& BinaryReader::stream() const
	{
		return istr_;
	}


	inline BinaryReader::StreamByteOrder BinaryReader::byteOrder() const
	{
#if defined(TNG_BIGENDIAN)
		return flipBytes_ ? LITTLE_ENDIAN_BYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
#else
		return flipBytes_ ? BIG_ENDIAN_BYTE_ORDER : LITTLE_ENDIAN_BYTE_ORDER;
#endif
	}


	inline void BinaryReader::setExceptions(std::ios_base::iostate st)
	{
		istr_.exceptions(st);
	}


	inline std::streamsize BinaryReader::available() const
	{
		return istr_.rdbuf()->in_avail();
	}

}



#endif