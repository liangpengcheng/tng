#pragma once
#include <vector>
#include "memobj.h"
#include "core/stringfun.h"
namespace tng
{

	class CORE_API Path :public MemObj
	{
	public:
		Path(const Path& p){ InitPath(p.str_utf8_path_); }
		Path(){}
		Path(const string& utf8Path);
		Path(const char* utf8Path);

		//append '/'at the end
		void				MakeAsDirectory();

		//append '/'at the end
		string				MakeAsDirectory()const;
		Path				RelativePath(const Path& mainPath)const;
		void				InitPath(const string& utf8Path);

		/// split to directory filename(no ext) ext
		void				Split(string& utf8_directory, string& utf8_filename, string& utf8_file_ext);
		///return utf8 string
		__inline operator	const string&()const{ return str_utf8_path_; }

		///if return true it has parent dir
		bool				ParentPath(Path& ret)const;

		/// get current application path not thread safe
		static Path			GetExePath();
		static Path			GetWorkPath();
		static void			SetWorkPath(const Path& p);
		bool				operator == (const Path& p)
		{
			return string(*this) == string(p);
		}
	private:
		void				convertToInternal(string& path);
		/// utf8 path
		string			str_utf8_path_;
#ifdef TNG_OS_FAMILY_WINDOWS
		/// wide string path
		wstring		str_utf16_path_;
	public:
		///return wide string (windows utf16 unix like utf32)
		__inline operator	const wstring&()const{ return str_utf16_path_; }
		/// accept windows or unix style path
		void				InitPath(const wstring& path);
		//not suggest to use
		Path(const wchar_t* path);
		//not suggest to use
		Path(const wstring& path);
		void				convertToInternal(wstring& path);
#endif // TNG_OS_FAMILY_WINDOWS


	};

}