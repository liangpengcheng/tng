#include "core/filestream.h"
#include "core/filesystem.h"
#include "core/runtime_check.h"
#include "core/stringfun.h"
namespace tng
{
#ifdef TNG_OS_FAMILY_WINDOWS

	FileStreamBuf::FileStreamBuf() :
		BufferedBidirectionalStreamBuf(BUFFER_SIZE, std::ios::in | std::ios::out),
		handle_(INVALID_HANDLE_VALUE),
		pos_(0)
	{
	}


	FileStreamBuf::~FileStreamBuf()
	{
		close();
	}


	void FileStreamBuf::open(const string& path, std::ios::openmode mode)
	{
		Assert(handle_ == INVALID_HANDLE_VALUE);

		path_ = path;
		pos_ = 0;
		setMode(mode);
		resetBuffers();

		DWORD access = 0;
		if (mode & std::ios::in)
			access |= GENERIC_READ;
		if (mode & std::ios::out)
			access |= GENERIC_WRITE;

		DWORD shareMode = FILE_SHARE_READ;
		if (!(mode & std::ios::out))
			shareMode |= FILE_SHARE_WRITE;

		DWORD creationDisp = OPEN_EXISTING;
		if (mode & std::ios::trunc)
			creationDisp = CREATE_ALWAYS;
		else if (mode & std::ios::out)
			creationDisp = OPEN_ALWAYS;

		DWORD flags = FILE_ATTRIBUTE_NORMAL;


		w16string utf16Path;
		ToWideString(path, utf16Path);
		handle_ = CreateFileW(utf16Path.c_str(), access, shareMode, NULL, creationDisp, flags, NULL);

		if (handle_ == INVALID_HANDLE_VALUE)
			FileSystem::handleFileSystemError(path_);

		if ((mode & std::ios::ate) || (mode & std::ios::app))
			seekoff(0, std::ios::end, mode);
	}


	int FileStreamBuf::readFromDevice(char* buffer, std::streamsize length)
	{
		if (INVALID_HANDLE_VALUE == handle_ || !(getMode() & std::ios::in))
			return -1;

		if (getMode() & std::ios::out)
			sync();

		DWORD bytesRead(0);
		BOOL rc = ReadFile(handle_, buffer, static_cast<DWORD>(length), &bytesRead, NULL);
		if (rc == 0)
			FileSystem::handleFileSystemError(path_);

		pos_ += bytesRead;

		return static_cast<int>(bytesRead);
	}


	int FileStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
	{
		if (INVALID_HANDLE_VALUE == handle_ || !(getMode() & std::ios::out))
			return -1;

		if (getMode() & std::ios::app)
		{
			LARGE_INTEGER li;
			li.QuadPart = 0;
			li.LowPart = SetFilePointer(handle_, li.LowPart, &li.HighPart, FILE_END);
			if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
				FileSystem::handleFileSystemError(path_);
			pos_ = li.QuadPart;
		}

		DWORD bytesWritten(0);
		BOOL rc = WriteFile(handle_, buffer, static_cast<DWORD>(length), &bytesWritten, NULL);
		if (rc == 0)
			FileSystem::handleFileSystemError(path_);

		pos_ += bytesWritten;

		return static_cast<int>(bytesWritten);
	}


	bool FileStreamBuf::close()
	{
		bool success = true;
		if (handle_ != INVALID_HANDLE_VALUE)
		{
			try
			{
				if (getMode() & std::ios::out)
					sync();
			}
			catch (...)
			{
				success = false;
			}
			CloseHandle(handle_);
			handle_ = INVALID_HANDLE_VALUE;
		}
		return success;
	}


	std::streampos FileStreamBuf::seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode)
	{
		if (INVALID_HANDLE_VALUE == handle_ || !(getMode() & mode))
			return -1;

		if (getMode() & std::ios::out)
			sync();

		std::streamoff adj;
		if (mode & std::ios::in)
			adj = static_cast<std::streamoff>(egptr() - gptr());
		else
			adj = 0;

		resetBuffers();

		DWORD offset = FILE_BEGIN;
		if (dir == std::ios::cur)
		{
			offset = FILE_CURRENT;
			off -= adj;
		}
		else if (dir == std::ios::end)
		{
			offset = FILE_END;
		}

		LARGE_INTEGER li;
		li.QuadPart = off;
		li.LowPart = SetFilePointer(handle_, li.LowPart, &li.HighPart, offset);

		if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
			FileSystem::handleFileSystemError(path_);
		pos_ = li.QuadPart;
		return std::streampos(static_cast<std::streamoff>(pos_));
	}


	std::streampos FileStreamBuf::seekpos(std::streampos pos, std::ios::openmode mode)
	{
		if (INVALID_HANDLE_VALUE == handle_ || !(getMode() & mode))
			return -1;

		if (getMode() & std::ios::out)
			sync();

		resetBuffers();

		LARGE_INTEGER li;
		li.QuadPart = pos;
		li.LowPart = SetFilePointer(handle_, li.LowPart, &li.HighPart, FILE_BEGIN);

		if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
			FileSystem::handleFileSystemError(path_);
		pos_ = li.QuadPart;
		return std::streampos(static_cast<std::streamoff>(pos_));
	}
#else
#if defined(TNG_ANDROID)

	FileStreamBufAPK::FileStreamBufAPK():
		BufferedBidirectionalStreamBuf(BUFFER_SIZE, std::ios::in | std::ios::out),
		asset_(NULL),
		pos_(0)
	{
	}


	FileStreamBufAPK::~FileStreamBufAPK()
	{
		close();
	}


	void FileStreamBufAPK::open(const string& path, std::ios::openmode mode)
	{
		Assert (asset_ == NULL);

		pos_ = 0;
		path_ = path;
		setMode(mode);
		resetBuffers();

		asset_ = AAssetManager_open(FileSystem::assetManager__, path_.c_str(), AASSET_MODE_UNKNOWN);
		if (!asset_)
		{
			Log::GetLog()->Printf(Log::WARNING_CHN,"can't find file : %s",path.c_str());
			FileSystem::handleFileSystemError(path_);
		}
		else
		{
			Log::GetLog()->Printf(Log::SYS_CHN,"open file : %s,%p",path.c_str(),asset_);
		}
		if ((mode & std::ios::app) || (mode & std::ios::ate))
			seekoff(0, std::ios::end, mode);
	}


	int FileStreamBufAPK::readFromDevice(char* buffer, std::streamsize length)
	{
		int result = AAsset_read(asset_, buffer, length);
		pos_+=result;
		return result;
	}


	int FileStreamBufAPK::writeToDevice(const char* buffer, std::streamsize length)
	{
		Log::GetLog()->Printf(Log::ERROR_CHN,"cant write data to apk file");
		return 0;
	}


	bool FileStreamBufAPK::close()
	{
		AAsset_close(asset_);
		asset_ = NULL;
		return 1;
	}


	std::streampos FileStreamBufAPK::seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode)
	{
		if (asset_ == NULL || !(getMode() & mode)) 
			return -1;

		std::streamoff adj;
		if (mode & std::ios::in)
			adj = static_cast<std::streamoff>(egptr() - gptr());
		else
			adj = 0;

		resetBuffers();

		int whence = SEEK_SET;
		if (dir == std::ios::cur)
		{
			whence = SEEK_CUR;
			off -= adj;
		}
		else if (dir == std::ios::end)
		{
			whence = SEEK_END;
		}
		pos_ = AAsset_seek(asset_, off, whence);
		return pos_;
	}


	std::streampos FileStreamBufAPK::seekpos(std::streampos pos, std::ios::openmode mode)
	{
		if (asset_ == NULL|| !(getMode() & mode)) 
			return -1;

		pos_ = AAsset_seek(asset_, pos, SEEK_SET);
		return pos_;
	}
#endif

	FileStreamBuf::FileStreamBuf():
		BufferedBidirectionalStreamBuf(BUFFER_SIZE, std::ios::in | std::ios::out),
		fd_(-1),
		pos_(0)
	{
	}


	FileStreamBuf::~FileStreamBuf()
	{
		close();
	}


	void FileStreamBuf::open(const string& path, std::ios::openmode mode)
	{
		Assert (fd_ == -1);

		pos_ = 0;
		path_ = path;
		setMode(mode);
		resetBuffers();

		int flags(0);
		if (mode & std::ios::trunc)
			flags |= O_TRUNC;
		if (mode & std::ios::app)
			flags |= O_APPEND;
		if (mode & std::ios::out)
			flags |= O_CREAT;
		if ((mode & std::ios::in) && (mode & std::ios::out))
			flags |= O_RDWR;
		else if (mode & std::ios::in)
			flags |= O_RDONLY;
		else
			flags |= O_WRONLY;

		fd_ = ::open(path.c_str(), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if (fd_ == -1)
			FileSystem::handleFileSystemError(path_);

		if ((mode & std::ios::app) || (mode & std::ios::ate))
			seekoff(0, std::ios::end, mode);
	}


	int FileStreamBuf::readFromDevice(char* buffer, std::streamsize length)
	{
		if (fd_ == -1) return -1;

		if (getMode() & std::ios::out)
			sync();

		int n = read(fd_, buffer, length);
		if (n == -1)
			FileSystem::handleFileSystemError(path_);
		pos_ += n;
		return n;
	}


	int FileStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
	{
		if (fd_ == -1) return -1;

		int n = write(fd_, buffer, length);

		if (n == -1)
			FileSystem::handleFileSystemError(path_);
		pos_ += n;
		return n;
	}


	bool FileStreamBuf::close()
	{
		bool success = true;
		if (fd_ != -1)
		{
			try
			{
				sync();
			}
			catch (...)
			{
				success = false;
			}
			::close(fd_);
			fd_ = -1;
		}
		return success;
	}


	std::streampos FileStreamBuf::seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode)
	{
		if (fd_ == -1 || !(getMode() & mode)) 
			return -1;

		if (getMode() & std::ios::out)
			sync();

		std::streamoff adj;
		if (mode & std::ios::in)
			adj = static_cast<std::streamoff>(egptr() - gptr());
		else
			adj = 0;

		resetBuffers();

		int whence = SEEK_SET;
		if (dir == std::ios::cur)
		{
			whence = SEEK_CUR;
			off -= adj;
		}
		else if (dir == std::ios::end)
		{
			whence = SEEK_END;
		}
		pos_ = lseek(fd_, off, whence);
		return pos_;
	}


	std::streampos FileStreamBuf::seekpos(std::streampos pos, std::ios::openmode mode)
	{
		if (fd_ == -1 || !(getMode() & mode)) 
			return -1;

		if (getMode() & std::ios::out)
			sync();

		resetBuffers();

		pos_ = lseek(fd_, pos, SEEK_SET);
		return pos_;
	}

#endif // TNG_OS_FAMILY_WINDOWS

	FileIOS::FileIOS(std::ios::openmode defaultMode) :
		defaultMode_(defaultMode)
	{
	}


	FileIOS::~FileIOS()
	{
	}


	void FileIOS::open(const string& path, std::ios::openmode mode)
	{
		clear();
		buf_.open(path, mode | defaultMode_);
	}


	void FileIOS::close()
	{
		if (!buf_.close())
		{
			setstate(ios_base::badbit);
		}
	}


	FileStreamBuf* FileIOS::rdbuf()
	{
		return &buf_;
	}


	FileInputStream::FileInputStream() :
		FileIOS(std::ios::in),
		std::istream(&buf_)
	{
		}


	FileInputStream::FileInputStream(const string& path, std::ios::openmode mode) :
		FileIOS(std::ios::in),
		std::istream(&buf_)
	{
			open(path, mode);
		}


	FileInputStream::~FileInputStream()
	{
	}


	FileOutputStream::FileOutputStream() :
		FileIOS(std::ios::out),
		std::ostream(&buf_)
	{
		}


	FileOutputStream::FileOutputStream(const string& path, std::ios::openmode mode) :
		FileIOS(std::ios::out),
		std::ostream(&buf_)
	{
			open(path, mode);
		}


	FileOutputStream::~FileOutputStream()
	{
	}


	FileStream::FileStream() :
		FileIOS(std::ios::in | std::ios::out),
		std::iostream(&buf_)
	{
		}


	FileStream::FileStream(const string& path, std::ios::openmode mode) :
		FileIOS(std::ios::in | std::ios::out),
		std::iostream(&buf_)
	{
			open(path, mode);
		}


	FileStream::~FileStream()
	{
	}

#ifdef TNG_ANDROID

	FileIOSAPK::FileIOSAPK(std::ios::openmode defaultMode):
		defaultMode_(defaultMode)
	{
	}


	FileIOSAPK::~FileIOSAPK()
	{
	}


	void FileIOSAPK::open(const string& path, std::ios::openmode mode)
	{
		clear();
		buf_.open(path, mode | defaultMode_);
	}


	void FileIOSAPK::close()
	{
		if (!buf_.close())
		{
			setstate(ios_base::badbit);
		}
	}


	FileStreamBufAPK* FileIOSAPK::rdbuf()
	{
		return &buf_;
	}


	FileInputStreamAPK::FileInputStreamAPK():
		FileIOSAPK(std::ios::in),
		std::istream(&buf_)
	{
		}


	FileInputStreamAPK::FileInputStreamAPK(const string& path, std::ios::openmode mode):
		FileIOSAPK(std::ios::in),
		std::istream(&buf_)
	{
			open(path, mode);
		}


	FileInputStreamAPK::~FileInputStreamAPK()
	{
	}

#endif
}