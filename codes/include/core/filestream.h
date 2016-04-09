#ifndef _FILESTREAM_H_
#define _FILESTREAM_H_
#include "core/stl_rebind.h"
#include "arch_detect.h"
#include <streambuf>
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>
#include "buffer_alloc.h"
#ifdef TNG_OS_FAMILY_WINDOWS
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif // TNG_OS_FAMILY_WINDOWS
#ifdef TNG_ANDROID
#include <android/asset_manager.h>
#endif

#include "types.h"
#include "core_macros.h"
namespace tng
{
	static __inline void stream_copy_n(std::istream & in, std::size_t count, std::ostream & out)
	{
		const std::size_t buffer_size = 4096;
		char buffer[buffer_size];
		while (count > buffer_size)
		{
			in.read(buffer, buffer_size);
			out.write(buffer, buffer_size);
			count -= buffer_size;
		}

		in.read(buffer, count);
		out.write(buffer, count);
	}
	template <typename ch, typename tr, typename ba = BufferAllocator<ch> >
	class BasicBufferedBidirectionalStreamBuf : public std::basic_streambuf<ch, tr>
		/// This is an implementation of a buffered bidirectional 
		/// streambuf that greatly simplifies the implementation of
		/// custom streambufs of various kinds.
		/// Derived classes only have to override the methods
		/// readFromDevice() or writeToDevice().
		///
		/// In contrast to BasicBufferedStreambuf, this class supports
		/// simultaneous read and write access, so in addition to
		/// istream and ostream this streambuf can also be used
		/// for implementing an iostream.
	{
	protected:
		typedef std::basic_streambuf<ch, tr> Base;
		typedef std::basic_ios<ch, tr> IOS;
		typedef ch char_type;
		typedef tr char_traits;
		typedef ba Allocator;
		typedef typename Base::int_type int_type;
		typedef typename Base::pos_type pos_type;
		typedef typename Base::off_type off_type;
		typedef typename IOS::openmode openmode;

	public:
		BasicBufferedBidirectionalStreamBuf(std::streamsize bufferSize, openmode mode) :
			bufsize_(bufferSize),
			pReadBuffer_(Allocator::allocate(bufsize_)),
			pWriteBuffer_(Allocator::allocate(bufsize_)),
			mode_(mode)
		{
			resetBuffers();
		}

		~BasicBufferedBidirectionalStreamBuf()
		{
			Allocator::deallocate(pReadBuffer_, bufsize_);
			Allocator::deallocate(pWriteBuffer_, bufsize_);
		}

		virtual int_type overflow(int_type c)
		{
			if (!(mode_ & IOS::out)) return char_traits::eof();

			if (c != char_traits::eof())
			{
				*this->pptr() = char_traits::to_char_type(c);
				this->pbump(1);
			}
			if (flushBuffer() == std::streamsize(-1)) return char_traits::eof();

			return c;
		}

		virtual int_type underflow()
		{
			if (!(mode_ & IOS::in)) return char_traits::eof();

			if (this->gptr() && (this->gptr() < this->egptr()))
				return char_traits::to_int_type(*this->gptr());

			int putback = int(this->gptr() - this->eback());
			if (putback > 4) putback = 4;

			char_traits::move(pReadBuffer_ + (4 - putback), this->gptr() - putback, putback);

			int n = readFromDevice(pReadBuffer_ + 4, bufsize_ - 4);
			if (n <= 0) return char_traits::eof();

			this->setg(pReadBuffer_ + (4 - putback), pReadBuffer_ + 4, pReadBuffer_ + 4 + n);

			// return next character
			return char_traits::to_int_type(*this->gptr());
		}

		virtual int sync()
		{
			if (this->pptr() && this->pptr() > this->pbase())
			{
				if (flushBuffer() == -1) return -1;
			}
			return 0;
		}

	protected:
		void setMode(openmode mode)
		{
			mode_ = mode;
		}

		openmode getMode() const
		{
			return mode_;
		}

		void resetBuffers()
		{
			this->setg(pReadBuffer_ + 4, pReadBuffer_ + 4, pReadBuffer_ + 4);
			this->setp(pWriteBuffer_, pWriteBuffer_ + (bufsize_ - 1));
		}

	private:
		virtual int readFromDevice(char_type* buffer, std::streamsize length)
		{
			return 0;
		}

		virtual int writeToDevice(const char_type* buffer, std::streamsize length)
		{
			return 0;
		}

		int flushBuffer()
		{
			int n = int(this->pptr() - this->pbase());
			if (writeToDevice(this->pbase(), n) == n)
			{
				this->pbump(-n);
				return n;
			}
			return -1;
		}

		std::streamsize bufsize_;
		char_type*      pReadBuffer_;
		char_type*      pWriteBuffer_;
		openmode        mode_;

		BasicBufferedBidirectionalStreamBuf(const BasicBufferedBidirectionalStreamBuf&);
		BasicBufferedBidirectionalStreamBuf& operator = (const BasicBufferedBidirectionalStreamBuf&);
	};
	typedef BasicBufferedBidirectionalStreamBuf<char, std::char_traits<char> > BufferedBidirectionalStreamBuf;
#ifdef TNG_OS_FAMILY_WINDOWS
	class CORE_API FileStreamBuf : public BufferedBidirectionalStreamBuf
		/// This stream buffer handles Fileio
	{
	public:
		FileStreamBuf();
		/// Creates a FileStreamBuf.

		~FileStreamBuf();
		/// Destroys the FileStream.

		void open(const string& path, std::ios::openmode mode);
		/// Opens the given file in the given mode.

		bool close();
		/// Closes the File stream buffer. Returns true if successful,
		/// false otherwise.

		std::streampos seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// change position by offset, according to way and mode

		std::streampos seekpos(std::streampos pos, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// change to specified position, according to mode

	protected:
		enum
		{
			BUFFER_SIZE = 4096
		};

		int readFromDevice(char* buffer, std::streamsize length);
		int writeToDevice(const char* buffer, std::streamsize length);

	private:
		string		path_;
		HANDLE			handle_;
		u64				pos_;
	};
#else
#if defined(TNG_ANDROID)
	class CORE_API FileStreamBufAPK: public BufferedBidirectionalStreamBuf
		/// This stream buffer handles Fileio
	{
	public:
		FileStreamBufAPK();
		/// Creates a FileStreamBuf.

		~FileStreamBufAPK();
		/// Destroys the FileStream.

		void open(const string& path, std::ios::openmode mode);
		/// Opens the given file in the given mode.

		bool close();
		/// Closes the File stream buffer. Returns true if successful,
		/// false otherwise.

		std::streampos seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// Change position by offset, according to way and mode.

		std::streampos seekpos(std::streampos pos, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// Change to specified position, according to mode.

	protected:
		enum
		{
			BUFFER_SIZE = 4096
		};

		int readFromDevice(char* buffer, std::streamsize length);
		int writeToDevice(const char* buffer, std::streamsize length);

	private:
		string path_;
		AAsset* asset_;
		std::streamoff pos_;
	};
#endif
	class CORE_API FileStreamBuf: public BufferedBidirectionalStreamBuf
		/// This stream buffer handles Fileio
	{
	public:
		FileStreamBuf();
		/// Creates a FileStreamBuf.

		~FileStreamBuf();
		/// Destroys the FileStream.

		void open(const string& path, std::ios::openmode mode);
		/// Opens the given file in the given mode.

		bool close();
		/// Closes the File stream buffer. Returns true if successful,
		/// false otherwise.

		std::streampos seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// Change position by offset, according to way and mode.

		std::streampos seekpos(std::streampos pos, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// Change to specified position, according to mode.

	protected:
		enum
		{
			BUFFER_SIZE = 4096
		};

		int readFromDevice(char* buffer, std::streamsize length);
		int writeToDevice(const char* buffer, std::streamsize length);

	private:
		string path_;
		int fd_;
		std::streamoff pos_;
	};


#endif // TNG_OS_FAMILY_WINDOWS
	class CORE_API FileIOS : public virtual std::ios

	{
	public:
		FileIOS(std::ios::openmode defaultMode);


		~FileIOS();

		/**
			Accept utf8 path
			*/
		void open(const string& path, std::ios::openmode mode);


		void close();

		FileStreamBuf* rdbuf();

	protected:
		FileStreamBuf buf_;
		std::ios::openmode defaultMode_;
	};


	class CORE_API FileInputStream : public FileIOS, public std::istream
	{
	public:
		FileInputStream();

		/**
			Accept utf8 path
			*/
		FileInputStream(const string& path, std::ios::openmode mode = std::ios::in);


		~FileInputStream();

	};


	class CORE_API FileOutputStream : public FileIOS, public std::ostream
	{
	public:
		FileOutputStream();

		/**
			Accept utf8 path
			*/
		FileOutputStream(const string& path, std::ios::openmode mode = std::ios::out | std::ios::trunc);


		~FileOutputStream();

	};


	class CORE_API FileStream : public FileIOS, public std::iostream

	{
	public:
		FileStream();

		/**
			Accept utf8 path
			*/
		FileStream(const string& path, std::ios::openmode mode = std::ios::out | std::ios::in);


		~FileStream();

	};

#ifdef TNG_ANDROID
	class CORE_API FileIOSAPK: public virtual std::ios
	{
	public:
		FileIOSAPK(std::ios::openmode defaultMode);


		~FileIOSAPK();

		/**
			Accept utf8 path
			*/
		void open(const string& path, std::ios::openmode mode);


		void close();

		FileStreamBufAPK* rdbuf();

	protected:
		FileStreamBufAPK buf_;
		std::ios::openmode defaultMode_;
	};


	class CORE_API FileInputStreamAPK: public FileIOSAPK, public std::istream
	{
	public:
		FileInputStreamAPK();

		/**
			Accept utf8 path
			*/
		FileInputStreamAPK(const string& path, std::ios::openmode mode = std::ios::in);


		~FileInputStreamAPK();

	};

#endif // TNG_ANDROID

}

#endif