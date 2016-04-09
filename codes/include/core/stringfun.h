#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "core/core_macros.h"
#include "core/exception.h"
#include "core/types.h"
#include "core/stl_rebind.h"
#ifdef __ANDROID__

namespace std
{
	typedef basic_string<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wstring;

	typedef basic_ios<wchar_t, char_traits<wchar_t> > wios;
	typedef basic_streambuf<wchar_t, char_traits<wchar_t> >
		wstreambuf;
	typedef basic_istream<wchar_t, char_traits<wchar_t> > wistream;
	typedef basic_ostream<wchar_t, char_traits<wchar_t> > wostream;
	typedef basic_iostream<wchar_t, char_traits<wchar_t> > wiostream;
	typedef basic_stringbuf<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wstringbuf;
	typedef basic_istringstream<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wistringstream;
	typedef basic_ostringstream<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wostringstream;
	typedef basic_stringstream<wchar_t, char_traits<wchar_t>,
		allocator<wchar_t> > wstringstream;
	typedef basic_filebuf<wchar_t, char_traits<wchar_t> > wfilebuf;
	typedef basic_ifstream<wchar_t, char_traits<wchar_t> > wifstream;
	typedef basic_ofstream<wchar_t, char_traits<wchar_t> > wofstream;
	typedef basic_fstream<wchar_t, char_traits<wchar_t> > wfstream;

}
#endif
namespace tng
{

#ifdef _WIN32
#define vsnwprintf _vsnwprintf
#else
#define vsnwprintf vswprintf
#endif

	inline bool IsAlpha(char ch) {
		return isalpha(static_cast<unsigned char>(ch)) != 0;
	}
	inline bool IsAlNum(char ch) {
		return isalnum(static_cast<unsigned char>(ch)) != 0;
	}
	inline bool IsDigit(char ch) {
		return isdigit(static_cast<unsigned char>(ch)) != 0;
	}
	inline bool IsLower(char ch) {
		return islower(static_cast<unsigned char>(ch)) != 0;
	}
	inline bool IsSpace(char ch) {
		return isspace(static_cast<unsigned char>(ch)) != 0;
	}
	inline bool IsUpper(char ch) {
		return isupper(static_cast<unsigned char>(ch)) != 0;
	}
	inline bool IsXDigit(char ch) {
		return isxdigit(static_cast<unsigned char>(ch)) != 0;
	}

	inline char ToLower(char ch) {
		return static_cast<char>(tolower(static_cast<unsigned char>(ch)));
	}
	inline char ToUpper(char ch) {
		return static_cast<char>(toupper(static_cast<unsigned char>(ch)));
	}

	__inline string str(const string& org)
	{
		return org;
	}

	class format
	{
	public:
		format(const string& _d)
			: d(_d)
		{
		}

		template <typename T>
		format& operator % (T in)
		{
			ostringstream ss;
			ss << in;
			chunks.push_back(ss.str());
			return *this;
		}


		operator string () const {
			string res; // pray for NRVO to kick in

			size_t start = 0, last = 0;

			vector<string>::const_iterator chunkin = chunks.begin();

			for (start = d.find('%'); start != string::npos; start = d.find('%', last)) {
				res += d.substr(last, start - last);
				last = start + 2;
				if (d[start + 1] == '%') {
					res += "%";
					continue;
				}

				if (chunkin == chunks.end()) {
					break;
				}

				res += *chunkin++;
			}
			res += d.substr(last);
			return res;
		}

	private:
		string d;
		vector<string> chunks;
	};

	__inline void split(const string& s, const  string& delim, vector< string >* ret)
	{
		size_t last = 0;
		size_t index = s.find_first_of(delim, last);
		while (index != string::npos)
		{
			ret->push_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
		if (index - last > 0)
		{
			ret->push_back(s.substr(last, index - last));
		}
	}

	__inline string join(const vector<string>& strarr, string delimiter)
	{
		if (strarr.size() > 0)
		{
			string ret = strarr[0];
			for (s32 i = 1; i < strarr.size(); i++)
			{
				ret = ret + delimiter + strarr[i];
			}
			return ret;
		}
		return "";

	}

	CORE_GLOBAL_API void ToLower(string& strinout);
	CORE_GLOBAL_API string Format(const char* fmt ...);

	/// Converts the given UTF-8 encoded string into an UTF-16 encoded wstring.
	CORE_GLOBAL_API void ToWideString(const a8string& utf8String, w16string& wideString);
	/// Converts the given UTF-8 encoded string into an UTF-32 encoded wstring.
	CORE_GLOBAL_API void ToWideString(const a8string& utf8String, w32string& wideString);

	/// Converts the given UTF-16 encoded wstring into an UTF-8 encoded string.
	CORE_GLOBAL_API void ToUTF8(const w16string& wideString, a8string& utf8String);

	/// Converts the given UTF-32 encoded wstring into an UTF-8 encoded string.
	CORE_GLOBAL_API void ToUTF8(const w32string& wideString, a8string& utf8String);
}
