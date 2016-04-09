#include "core/table_paser.h"
namespace tng
{
#define LINE_DATA_MAX 16383
	static char LineData[LINE_DATA_MAX];
	int TablePaser::GetColumn(stringstream& ss_str, std::basic_istream<char>& in_put, string& column)
	{
		column = "";
		if (ss_str.eof())
			return -1;
		char cur_char[2];
		cur_char[1] = 0;
		char separator = '	';

		if (ss_str.get(cur_char[0]).eof())
			return 1;
		if (cur_char[0] == '	')
			return 1;
		else if (cur_char[0] == '"')
			separator = '"';
		else
			column += string(cur_char);


		while (1)
		{
			if (ss_str.eof())
			{
				if (separator == '"')
				{
					if (in_put.eof())
						return 0;

					column += string("\n\0");
					ss_str.clear();
					in_put.getline(LineData, LINE_DATA_MAX);
					ss_str << LineData;
				}
				else
					return 0;
			}
			ss_str.getline(LineData, 4096, separator);
			column += LineData;
			//column.append(LineData);
			if (ss_str.eof())
				continue;
			if (separator == '	')
				return 1;
			else if (separator == '"')
			{
				if (ss_str.eof())
					return 0;
				ss_str.get(cur_char[0]);
				if (ss_str.eof())
					return 1;
				else if (cur_char[0] == '	')
					return 1;
				else if (cur_char[0] != separator)
					separator = '	';
				//column.push_back(cur_char);
				column += string(cur_char);
			}
		}
		return -1;
	}

	int TablePaser::GetLine(std::basic_istream<char>& in_put, vector<string>& columns)
	{
		if (in_put.eof())
			return -1;

		std::streamoff i = in_put.tellg();
		memset(LineData, 0, LINE_DATA_MAX);
		in_put.getline(LineData, LINE_DATA_MAX);
		char* r_ch = strchr(LineData, '\r');
		if (r_ch)
		{
			in_put.seekg(i + (r_ch - LineData) + 1);
			*r_ch = '\0';
		}

		if (strlen(LineData) == 0)
			return GetLine(in_put, columns);

		stringstream ss_str;
		ss_str << LineData;

		int ret = 1;
		string column;
		while (ret == 1)
		{
			if ((ret = GetColumn(ss_str, in_put, column)) == -1)
				break;
			columns.push_back(column);
		}
		if (in_put.eof())
			return 0;
		return 1;
	}

	bool TablePaser::LoadTable(std::basic_istream<char>& in_put)
	{

		vector<string> headerColumns;

		int ret = GetLine(in_put, headerColumns);



		ColumnNames.clear();
		vector<string>::iterator iterBegin = headerColumns.begin();
		vector<string>::iterator iterEnd = headerColumns.end();
		for (u8 i = 0; iterBegin != iterEnd; ++iterBegin, ++i)
			ColumnNames[*iterBegin] = i;

		while (ret == 1)
		{

			TableRecord tr;
			if ((ret = GetLine(in_put, tr)) != -1)
			{
				Records.push_back(tr);
			}
			else
			{
				break;
			}
			//iterBegin = Records.back().begin();
		}
		return true;
	}


	bool TablePaser::LoadTableFile(const char* fileName)
	{
		std::ifstream in_put;
		in_put.open(fileName);
		//FEAssert(in_put);
		bool ret = LoadTable(in_put);
		in_put.close();
		return ret;
	}

	void TablePaser::GetStrElements(const char* lineData, vector<string>& strElements, const char sperator)
	{
		strElements.clear();
		char* tData = const_cast<char*>(lineData);
		char* cBegin = tData;
		char* cEnd = strchr(cBegin, '\r');
		if (cEnd)
			*cEnd = '\0';
		cEnd = tData;
		for (; cEnd; cBegin = cEnd + 1)
		{
			cEnd = strchr(cBegin, sperator);
			if (NULL != cEnd)
				*cEnd = '\0';
			strElements.push_back(cBegin);
			if (NULL != cEnd)
				*cEnd = sperator;
		}
	}
}