#ifndef _TNG_MEMSTREAM_H_
#define _TNG_MEMSTREAM_H_
#include <streambuf>
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include "core/core_macros.h"
namespace tng
{

	template <typename ch, typename tr> 
	class BasicMemoryStreamBuf: public std::basic_streambuf<ch, tr>
	{
	protected:
		typedef std::basic_streambuf<ch, tr> Base;
		typedef std::basic_ios<ch, tr> IOS;
		typedef ch char_type;
		typedef tr char_traits;
		typedef typename Base::int_type int_type;
		typedef typename Base::pos_type pos_type;
		typedef typename Base::off_type off_type;

	public:
		BasicMemoryStreamBuf(char_type* pBuffer, std::streamsize bufferSize):
			buffer_(pBuffer),
			buffer_size_(bufferSize)
		{
			this->setg(buffer_, buffer_, buffer_ + buffer_size_);
			this->setp(buffer_, buffer_ + buffer_size_);
		}

		~BasicMemoryStreamBuf()
		{
		}

		virtual int_type overflow(int_type c)
		{
			return char_traits::eof();
		}

		virtual int_type underflow()
		{
			return char_traits::eof();
		}

		virtual int sync()
		{
			return 0;
		}

		std::streamsize charsWritten() const
		{
			return static_cast<std::streamsize>(this->pptr() - this->pbase());
		}

		void reset()
			/// Resets the buffer so that current read and write positions
			/// will be set to the beginning of the buffer.
		{
			this->setg(buffer_, buffer_, buffer_ + buffer_size_);
			this->setp(buffer_, buffer_ + buffer_size_);
		}

	private:
		char_type*      buffer_;
		std::streamsize buffer_size_;

		BasicMemoryStreamBuf();
		BasicMemoryStreamBuf(const BasicMemoryStreamBuf&);
		BasicMemoryStreamBuf& operator = (const BasicMemoryStreamBuf&);
	};


	//
	// We provide an instantiation for char
	//
	typedef BasicMemoryStreamBuf<char, std::char_traits<char> > MemoryStreamBuf;


	class CORE_API MemoryIOS: public virtual std::ios
		/// The base class for MemoryInputStream and MemoryOutputStream.
		///
		/// This class is needed to ensure the correct initialization
		/// order of the stream buffer and base classes.
	{
	public:
		MemoryIOS(char* pBuffer, std::streamsize bufferSize);
		/// Creates the basic stream.

		~MemoryIOS();
		/// Destroys the stream.

		MemoryStreamBuf* rdbuf();
		/// Returns a pointer to the underlying streambuf.

	protected:
		MemoryStreamBuf buf_;
	};


	class CORE_API MemoryInputStream: public MemoryIOS, public std::istream
		/// An input stream for reading from a memory area.
	{
	public:
		MemoryInputStream(const char* pBuffer, std::streamsize bufferSize);
		/// Creates a MemoryInputStream for the given memory area,
		/// ready for reading.

		~MemoryInputStream();
		/// Destroys the MemoryInputStream.
	};


	class CORE_API MemoryOutputStream: public MemoryIOS, public std::ostream
		/// An input stream for reading from a memory area.
	{
	public:
		MemoryOutputStream(char* pBuffer, std::streamsize bufferSize);
		/// Creates a MemoryOutputStream for the given memory area,
		/// ready for writing.

		~MemoryOutputStream();
		/// Destroys the MemoryInputStream.

		std::streamsize charsWritten() const;
		/// Returns the number of chars written to the buffer.
	};


	//
	// inlines
	//
	inline MemoryStreamBuf* MemoryIOS::rdbuf()
	{
		return &buf_;
	}


	inline std::streamsize MemoryOutputStream::charsWritten() const
	{
		return buf_.charsWritten();
	}

	MemoryIOS::MemoryIOS(char* pBuffer, std::streamsize bufferSize):
		buf_(pBuffer, bufferSize)
	{
	}


	MemoryIOS::~MemoryIOS()
	{
	}


	MemoryInputStream::MemoryInputStream(const char* pBuffer, std::streamsize bufferSize): 
		MemoryIOS(const_cast<char*>(pBuffer), bufferSize), 
		std::istream(&buf_)
	{
	}


	MemoryInputStream::~MemoryInputStream()
	{
	}


	MemoryOutputStream::MemoryOutputStream(char* pBuffer, std::streamsize bufferSize): 
		MemoryIOS(pBuffer, bufferSize), 
		std::ostream(&buf_)
	{
	}


	MemoryOutputStream::~MemoryOutputStream()
	{
	}

}

#endif