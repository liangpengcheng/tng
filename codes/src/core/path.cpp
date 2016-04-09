#include "uv/include/uv.h"
#include "core/runtime_check.h"
#include "core/path.h"
#include "utf8/utf8.h"
#ifdef _WIN32
#include "windows.h"
#else
#include "unistd.h"
#endif

#define INTERNALE_DEVID L'/'
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

namespace tng
{
	static char replace_devid[1] = {
		'\\'
	};
	Path::Path(const string& utf8Path)
	{
		InitPath(utf8Path);
	}

	Path::Path(const char* utf8Path)
	{
		InitPath(utf8Path);
	}

	void Path::MakeAsDirectory()
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		if (!str_utf16_path_.length()
			|| str_utf16_path_[str_utf16_path_.length() - 1] != '/')
		{
			str_utf16_path_.push_back(L'/');
			str_utf8_path_.push_back('/');
		}
#else
		if(!str_utf8_path_.length()
			||str_utf8_path_[str_utf8_path_.length()-1]!='/')
		{
			str_utf8_path_.push_back('/');
		}
#endif // TNG_OS_FAMILY_WINDOWS

	}
	string Path::MakeAsDirectory()const
	{
		string ret = str_utf8_path_;
		if (!ret.length()
			|| ret[ret.length() - 1] != '/')
		{
			ret.push_back('/');
		}
		return ret;
	}
	void Path::InitPath(const string& utf8Path)
	{
#ifdef _DEBUG
		string::const_iterator end_it = utf8::find_invalid(utf8Path.begin(), utf8Path.end());
		Assert(end_it == utf8Path.end())
#endif // _DEBUG
			str_utf8_path_ = utf8Path;
		convertToInternal(str_utf8_path_);
#ifdef TNG_OS_FAMILY_WINDOWS
		str_utf16_path_.clear();
		ToWideString(str_utf8_path_, str_utf16_path_);
#endif
	}

	void Path::Split(string& utf8_directory, string& utf8_filename, string& utf8_file_ext)
	{
		size_t extpos = str_utf8_path_.find_last_of('.');
		size_t dirpos = str_utf8_path_.find_last_of('/');
		utf8_directory = str_utf8_path_.substr(0, dirpos + 1);
		utf8_filename = str_utf8_path_.substr(dirpos + 1, extpos - (dirpos + 1));
		utf8_file_ext = str_utf8_path_.substr(extpos + 1, str_utf8_path_.length() - (extpos + 1));
	}

	Path Path::GetExePath()
	{
		static char maxpath[MAX_PATH * 2];
		static size_t maxsize = MAX_PATH * 2;
		uv_exepath(maxpath, &maxsize);
		return Path(maxpath);
	}
	Path Path::GetWorkPath()
	{
		static char maxpath[MAX_PATH * 2];
		static size_t maxsize = MAX_PATH * 2;

#ifdef TNG_OS_FAMILY_WINDOWS
		uv_cwd(maxpath, maxsize);
#else
		getcwd(maxpath, maxsize);
#endif // DEBUG


		return Path(maxpath);
	}
	void Path::SetWorkPath(const Path& p)
	{
#ifdef TNG_OS_FAMILY_WINDOWS
		uv_chdir(string(p).c_str());
#else
		chdir(string(p).c_str());
#endif // TNG_OS_FAMILY_WINDOWS

	}
#ifdef TNG_OS_FAMILY_WINDOWS
	Path::Path(const wstring& path)
	{
		InitPath(path);
	}
	void Path::convertToInternal(wstring& path)
	{
		wstring::iterator itstart = path.begin();
		wstring::iterator itend = path.end();

		for (wstring::iterator it = itstart;
			it != itend;
			it++)
		{
			for (size_t i = 0; i < sizeof(replace_devid); i++)
			if (*it == replace_devid[i])
				*it = INTERNALE_DEVID;
		}
	}
	Path::Path(const wchar_t* path)
	{
		InitPath(path);
	}
	void Path::InitPath(const wstring& path)
	{
		str_utf16_path_ = path;
		convertToInternal(str_utf16_path_);
		str_utf8_path_.clear();
		ToUTF8(str_utf16_path_, str_utf8_path_);
	}
#endif

	void Path::convertToInternal(string& path)
	{
		string::iterator itstart = path.begin();
		string::iterator itend = path.end();

		for (string::iterator it = itstart;
			it != itend;
			it++)
		{
			for (size_t i = 0; i < sizeof(replace_devid); i++)
			if (*it == replace_devid[i])
				*it = INTERNALE_DEVID;
		}
	}

	bool Path::ParentPath(Path& ret)const
	{
		s32 sz = (s32)str_utf8_path_.length();
		if (sz>1)
		{
			for (s32 i = sz - 2;
				i >= 0;
				i--)
			{
				if (str_utf8_path_[i] == INTERNALE_DEVID)
				{
					ret = Path(str_utf8_path_.substr(0, i));
					return true;;
				}
				if (i == 0)
					return false;
			}
		}

		return false;
	}

	Path Path::RelativePath(const Path& mainPath)const
	{
		string p = mainPath.MakeAsDirectory();
		return str_utf8_path_.substr(p.length(), str_utf8_path_.length());
	}
}