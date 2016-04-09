#include "core/stringfun.h"
#include "utf8/utf8.h"
#include "core/runtime_check.h"
#include "core/types.h"
#include "stdarg.h"
#include "stdio.h"
#include <algorithm>
#ifdef _WIN32
#pragma warning (disable:4996)
#endif
namespace tng
{

	string vformat(const char *fmt, va_list argPtr) {
		// We draw the line at a 1MB string.
		const int maxSize = 1000000;

		// If the string is less than 161 characters,
		// allocate it on the stack because this saves
		// the malloc/free time.
		const int bufSize = 161;
		char stackBuffer[bufSize];

		int attemptedSize = bufSize - 1;

		int numChars = vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);

		if (numChars >= 0) {
			// Got it on the first try.
			return string(stackBuffer);
		}

		// Now use the heap.
		char* heapBuffer = NULL;

		while ((numChars == -1) && (attemptedSize < maxSize)) {
			// Try a bigger size
			attemptedSize *= 2;
			char* oldb = heapBuffer;
			heapBuffer = (char*)realloc(heapBuffer, attemptedSize + 1);
			if(!heapBuffer){
				free(oldb);
			}
			numChars = vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
		}

		string result = string(heapBuffer);

		free(heapBuffer);

		return result;

	}


	// std::wstring vformat(const wchar_t *fmt, va_list argPtr) {
	//     // We draw the line at a 1MB string.
	//     const int maxSize = 1000000;
	// 
	//     // If the string is less than 161 characters,
	//     // allocate it on the stack because this saves
	//     // the malloc/free time.
	//     const int bufSize = 161;
	//     wchar_t stackBuffer[bufSize];
	// 
	//     int attemptedSize = (bufSize - 1)*sizeof(wchar_t);
	// 
	//     int numChars = vsnwprintf(stackBuffer, attemptedSize, fmt, argPtr);
	// 
	//     if (numChars >= 0) {
	//         // Got it on the first try.
	//         return std::wstring(stackBuffer);
	//     }
	// 
	//     // Now use the heap.
	//     wchar_t* heapBuffer = NULL;
	// 
	//     while ((numChars == -1) && (attemptedSize < maxSize)) {
	//         // Try a bigger size
	//         attemptedSize *= 2;
	//         heapBuffer = (wchar_t*)realloc(heapBuffer, attemptedSize + 1);
	//         numChars = vsnwprintf(heapBuffer, attemptedSize, fmt, argPtr);
	//     }
	// 
	//     std::wstring result(heapBuffer);
	// 
	//     free(heapBuffer);
	// 
	//     return result;
	// 
	// }

	string Format(const char* fmt ...)
	{
		va_list argList;
		va_start(argList, fmt);
		string result = vformat(fmt, argList);
		va_end(argList);

		return result;
	}
	// 
	// std::wstring Format(const wchar_t* fmt ...)
	// {
	//     va_list argList;
	//     va_start(argList, fmt);
	//     std::wstring result = vformat(fmt, argList);
	//     va_end(argList);
	// 
	//     return result;
	// }


	void ToWideString(const string& utf8String, w16string& utf16String)
	{
		string::const_iterator end_it = utf8::find_invalid(utf8String.begin(), utf8String.end());
#ifdef _DEBUG
		Assert(end_it == utf8String.end())
#endif // _DEBUG

			utf8::utf8to16(utf8String.begin(), end_it, back_inserter(utf16String));
	}

	void ToWideString(const string& utf8String, w32string& utf32String)
	{
		string::const_iterator end_it = utf8::find_invalid(utf8String.begin(), utf8String.end());
#ifdef _DEBUG
		Assert(end_it == utf8String.end())
#endif // _DEBUG

			utf8::utf8to16(utf8String.begin(), end_it, back_inserter(utf32String));
	}


	void ToUTF8(const w16string& wideString, a8string& utf8String)
	{
		utf8::utf16to8(wideString.begin(), wideString.end(), back_inserter(utf8String));

	}



	void ToUTF8(const w32string& wideString, a8string& utf8String)
	{
		utf8::utf16to8(wideString.begin(), wideString.end(), back_inserter(utf8String));

	}

	void ToLower(string& strinout)
	{
		std::transform(strinout.begin(), strinout.end(), strinout.begin(), ::tolower);
	}


}
