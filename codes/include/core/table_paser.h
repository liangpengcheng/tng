//TablePaser 
#ifndef _TABLEPASER_H_
#define _TABLEPASER_H_
#include "stringfun.h"
#include <fstream>
#include <sstream>
#include "runtime_check.h"
#include "hgl/Str.h"
#include "variant.h"
#include "core/stl_rebind.h"
#define LOAD_TABLE(table,filename)		\
	TablePaser table;					\
if (!table.LoadTableFile(filename))	\
	Assert(0);

namespace tng
{
	class CORE_API TablePaser
	{
	public:
		TablePaser(){}
		TablePaser(const char* fileName)
		{
			LoadTableFile(fileName);
		}

		bool LoadTableFile(const char* fileName);
		bool LoadTableString(const char* str)
		{
			stringstream ss_str(str);
			return LoadTable(ss_str);
		}

		u32 GetRecordsCounter(){ return (u32)Records.size(); }
		u32 GetColumnsCounter(){ return (u32)ColumnNames.size(); }

		s32 GetInt(u32 row, const char* columnName, s32 defaultVal = 0)
		{
			unordered_map<string, u8>::iterator MapIter = ColumnNames.find(columnName);
			Assert(MapIter != ColumnNames.end());
			const string& strData = GetItemData(row, MapIter->second);

			if (strData.length() == 0)
				return defaultVal;
			else
			{
				s32 val;
				hgl::etof(strData.c_str(), val);
				return val;
			}
		}
		f32 GetFloat(u32 row, const char* columnName, f32 defaultVal = 0.f)
		{
			unordered_map<string, u8>::iterator MapIter = ColumnNames.find(columnName);
			Assert(MapIter != ColumnNames.end());
			const string& strData = GetItemData(row, MapIter->second);

			if (strData.length() == 0)
				return defaultVal;
			else
			{
				f32 val;
				hgl::etof(strData.c_str(), val);
				return val;
			}
		}
		bool GetBool(u32 row, const char* columnName, bool defaultVal = true)
		{
			unordered_map<string, u8>::iterator MapIter = ColumnNames.find(columnName);
			Assert(MapIter != ColumnNames.end());
			const string& strData = GetItemData(row, MapIter->second);
			return (strData.length() == 0) ? defaultVal : (strcmp(strData.c_str(), "true") == 0 || strcmp(strData.c_str(), "1") == 0);
		}
		const string& GetString(u32 row, const char* columnName)
		{
			unordered_map<string, u8>::iterator MapIter = ColumnNames.find(columnName);
			Assert(MapIter != ColumnNames.end());
			return GetItemData(row, MapIter->second);
		}
		const string& GetItemData(u32 row, u32 col)
		{
			Assert(col < (u32)ColumnNames.size());
			Assert(row < (u32)Records.size());
			Assert(col < (u32)Records[row].size());
			return Records[row][col];
		}

		static void GetStrElements(const char* lineData, vector<string>& strElements, const char sperator);
	private:
		static int GetColumn(stringstream& ss_str, std::basic_istream<char>& in_put, string& column);
		static int GetLine(std::basic_istream<char>& in_put, vector<string>& columns);
		bool LoadTable(std::basic_istream<char>& in_put);

		typedef vector<string>	TableRecord;
		unordered_map<string, u8>			ColumnNames;
		vector<TableRecord>		Records;
	};

	//
	class TokenPaser
	{
	private:
		istringstream tokenStream;

	public:
		TokenPaser(const char* tokenStr) :tokenStream(tokenStr)
		{
			std::ws(tokenStream);
		}

		bool GetToken(string& token)
		{
			if (tokenStream.eof())
				return false;
			token = "";
			tokenStream >> token;
			return token.length() > 0;
		}
	};
}
#endif
