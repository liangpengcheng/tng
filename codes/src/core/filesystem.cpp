#include <algorithm>
#ifdef _WIN32
#include "windows.h"
#else
#include "errno.h"
#include <sys/stat.h>
#include <dirent.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "core/filesystem.h"
#include <iostream>
#ifdef TNG_OS_FAMILY_UNIX
#include "unistd.h"
#endif

namespace tng
{
#ifdef TNG_ANDROID
	AAssetManager* FileSystem::assetManager__ =NULL;
#endif // TNG_ANDROID
	void FileSystem::handleFileSystemError(const string& utf8str)
	{
#ifdef _WIN32

		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			throw FileNotFoundException(utf8str);
		case ERROR_PATH_NOT_FOUND:
		case ERROR_BAD_NETPATH:
		case ERROR_CANT_RESOLVE_FILENAME:
		case ERROR_INVALID_DRIVE:
			throw PathNotFoundException(utf8str);
		case ERROR_ACCESS_DENIED:
			throw FileAccessDeniedException(utf8str);
		case ERROR_ALREADY_EXISTS:
		case ERROR_FILE_EXISTS:
			throw FileExistsException(utf8str);
		case ERROR_INVALID_NAME:
		case ERROR_DIRECTORY:
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_BAD_PATHNAME:
			throw PathSyntaxException(utf8str);
		case ERROR_FILE_READ_ONLY:
			throw FileReadOnlyException(utf8str);
		case ERROR_CANNOT_MAKE:
			throw CreateFileException(utf8str);
		case ERROR_DIR_NOT_EMPTY:
			throw FileException("directory not empty", utf8str);
		case ERROR_WRITE_FAULT:
			throw WriteFileException(utf8str);
		case ERROR_READ_FAULT:
			throw ReadFileException(utf8str);
		case ERROR_SHARING_VIOLATION:
			throw FileException("sharing violation", utf8str);
		case ERROR_LOCK_VIOLATION:
			throw FileException("lock violation", utf8str);
		case ERROR_HANDLE_EOF:
			throw ReadFileException("EOF reached", utf8str);
		case ERROR_HANDLE_DISK_FULL:
		case ERROR_DISK_FULL:
			throw WriteFileException("disk is full", utf8str);
		case ERROR_NEGATIVE_SEEK:
			throw FileException("negative seek", utf8str);
		default:
			throw FileException(utf8str);
#else
		switch (errno)
		{
		case EIO:
			throw IOException(utf8str);
		case EPERM:
			throw FileAccessDeniedException("insufficient permissions", utf8str);
		case EACCES:
			throw FileAccessDeniedException(utf8str);
		case ENOENT:
			throw FileNotFoundException(utf8str);
		case ENOTDIR:
			throw OpenFileException("not a directory", utf8str);
		case EISDIR:
			throw OpenFileException("not a file", utf8str);
		case EROFS:
			throw FileReadOnlyException(utf8str);
		case EEXIST:
			throw FileExistsException(utf8str);
		case ENOSPC:
			throw FileException("no space left on device", utf8str);
		case EDQUOT:
			throw FileException("disk quota exceeded", utf8str);
#if !defined(_AIX)
		case ENOTEMPTY:
			throw FileException("directory not empty", utf8str);
#endif
		case ENAMETOOLONG:
			throw PathSyntaxException(utf8str);
		case ENFILE:
		case EMFILE:
			throw FileException("too many open files", utf8str);
		default:
#ifdef __ANDROID__
			throw FileException("unknown error", utf8str);
#elif (TNG_OS==TNG_OS_NET_BSD||TNG_OS==TNG_OS_DRAGONFLY)
			throw FileException(strerror(errno), utf8str);
#else
			throw FileException(strerror(errno), utf8str);
#endif

#endif
		}
	}
	size_t FileSystem::GetFileSize(const Path& path){
#ifdef _WIN32
		const wstring& filename = path;
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesExW(filename.c_str(), GetFileExInfoStandard, &fad) == 0)
			FileSystem::handleFileSystemError(path);
		LARGE_INTEGER li;
		li.LowPart = fad.nFileSizeLow;
		li.HighPart = fad.nFileSizeHigh;
		return size_t(li.QuadPart);
#else
		string utf8str= path;

		struct stat st;
		if (stat(utf8str.c_str(), &st) == 0)
			return st.st_size;
		else
			FileSystem::handleFileSystemError(path);
		return 0;
#endif

	}
	bool FileSystem::IsFileExist(const Path& path){
#ifdef _WIN32
		const wstring& filename = path;
		DWORD attr = GetFileAttributesW(filename.c_str());
		if (attr == 0xFFFFFFFF)
		{
			switch (GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
			case ERROR_NOT_READY:
			case ERROR_INVALID_DRIVE:
				return false;
			default:
				FileSystem::handleFileSystemError(path);
			}
		}
		return true;
#else
		string utf8str= path;

		struct stat st;
		return stat(utf8str.c_str(), &st) == 0;
#endif
	}
	bool FileSystem::CreateDir(const Path& path)
	{
#ifdef _WIN32
		const wstring& dirname = path;
		if (CreateDirectoryW(dirname.c_str(), 0) == 0)
			FileSystem::handleFileSystemError(path);
		return true;
#else
		string utf8str= path;
		if (mkdir(utf8str.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) 
			FileSystem::handleFileSystemError(path);
		return true;
#endif

	}
	bool FileSystem::IsLinkDir(const Path& path)
	{
#ifdef _WIN32
		return false;
#else
		string utf8str= path;
		struct stat st;
		if (lstat(utf8str.c_str(), &st) == 0)
			return S_ISLNK(st.st_mode);
		else
			FileSystem::handleFileSystemError(path);
		return false;
#endif
	}
	bool FileSystem::CreateDirs(const Path& path)
	{
		if (IsFileExist(path))
			return true;
		Path parent("");
		bool ret = 1;
		if (path.ParentPath(parent))
			ret &= CreateDirs(parent);
		ret &= CreateDir(path);
		return ret;
	}

	bool FileSystem::DeleteDir(const Path& path)
	{
#ifdef _WIN32
		const wstring& dirname = path;
		if (RemoveDirectoryW(dirname.c_str()) == 0)
		{
			FileSystem::handleFileSystemError(path);
			return false;
		}
		return true;
#else
		string utf8str= path;
		int rc;
		if (!IsLinkDir(path))
			rc = rmdir(utf8str.c_str());
		else
			rc = unlink(utf8str.c_str());
		if (rc) 
		{
			FileSystem::handleFileSystemError(path);
			return false;
		}
		return true;
#endif
	}

#ifdef TNG_ANDROID
	bool FileSystem::FindFilesInAPK(const Path& dir,vector<Path>& ret_paths)
	{
		string strpath(dir);
		AAssetDir* assetDir = AAssetManager_openDir(FileSystem::assetManager__,strpath.c_str());
		if (assetDir != NULL)
		{
			AAssetDir_rewind(assetDir);
			const char* file = NULL;
			while ((file = AAssetDir_getNextFileName(assetDir)) != NULL)
			{
				string filename(file);
				// Check if this file was already added to the list because it was copied to the SD card.
				if (find(ret_paths.begin(), ret_paths.end(), filename) == ret_paths.end())
				{
					ret_paths.push_back(dir.MakeAsDirectory()+filename);
				}
			}
			AAssetDir_close(assetDir);
		}

	}
#endif // TNG_ANDROID


	bool FileSystem::FindFiles(const Path& dir, vector<Path>& ret_paths, u32 filters /* = FileSystem::FFile|FileSystem::FDir */, bool recursive /* = 0 */)
	{
		Path current = dir;
		current.MakeAsDirectory();
#ifdef _WIN32
		WIN32_FIND_DATAW _fd;
		wstring findstr = current;
		findstr.push_back(L'*');
		HANDLE _fh = FindFirstFileW(findstr.c_str(), &_fd);
		if (_fh == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() != ERROR_NO_MORE_FILES)
				FileSystem::handleFileSystemError(current);
		}
		else
		{
			do
			{

				//if it is a directory
				if (_fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					if (wcscmp(_fd.cFileName, L".") && wcscmp(_fd.cFileName, L".."))
					{
						Path nextp = Path(wstring(current) + _fd.cFileName);
						nextp.MakeAsDirectory();
						if (recursive)
						{
							FindFiles(nextp, ret_paths, filters, recursive);
						}
						if (filters&FDir)
						{
							ret_paths.push_back(nextp);
						}
					}
				}
				else
				{
					if (filters&FFile)
					{
						wstring filepath(current);
						filepath.append(_fd.cFileName);
						ret_paths.push_back(filepath);
					}
				}
				_fd.cFileName;
			} while (FindNextFileW(_fh, &_fd));
		}


		if (_fh)
			FindClose(_fh);
#else
		string path= current;
		path.push_back('.');

		struct dirent* dp;
		DIR* pdir = opendir(path.c_str());
		if (!pdir)
		{
			FileSystem::handleFileSystemError(current);
			return false;
		}
		while ((dp = readdir(pdir)) != NULL)
		{
			string filepath(current);
			filepath.append(dp->d_name);

			struct stat buf;
			if (!stat(filepath.c_str(), &buf))
			{
				// Add to the list if this is not a directory
				if (!S_ISDIR(buf.st_mode))
				{
					if(FFile&filters)
						ret_paths.push_back(dp->d_name);
				}
				else
				{
					if(strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))
					{
						Path nextp = Path(string(current)+dp->d_name);
						nextp.MakeAsDirectory();
						if(recursive)
						{
							FindFiles(nextp,ret_paths,filters,recursive);
						}
						if (filters&FDir)
						{
							ret_paths.push_back(nextp);
						}
					}
				}
			}
		}
		closedir(pdir);

#endif
		return ret_paths.size() > 0 ? true : false;
	}
}