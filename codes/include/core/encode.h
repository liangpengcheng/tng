#ifndef _CORE_ENCODE_H_
#define _CORE_ENCODE_H_
#include "core_macros.h"
#include "core/types.h"
#include "core/binary_reader_writer.h"
#include "stl_rebind.h"
namespace tng
{

	class CORE_API Coder
	{
	public:
		virtual bool	Encode(std::istream& _in, std::ostream& _out) = 0;
		virtual bool	Decode(std::istream& _in, std::ostream& _out) = 0;
		static  void	SetDefaultCoder(Coder* coder){
			defaultCoder = coder;
		}
		static Coder*	GetDefaultCoder(){
			return defaultCoder;
		}
	protected:
		static Coder*			defaultCoder;
	};
	///not thread safe
	class CORE_API LZ4Coder :public Coder
	{
	public:
		virtual bool	Encode(std::istream& _in, std::ostream& _out)TNG_OVERRIDE;
		virtual bool	Decode(std::istream& _in, std::ostream& _out)TNG_OVERRIDE;
	};
	///RLE
	class CORE_API RLECompressor :public Coder
	{
	public:
		RLECompressor() :size_(1){}
		//默认是1格子节的run
		virtual bool		Encode(std::istream& _in, std::ostream& _out) TNG_OVERRIDE;
		virtual bool		Decode(std::istream& _in, std::ostream& _out) TNG_OVERRIDE;
		__inline void		SetRleSize(s32 size){ size_ = size; }
	private:
		s32 size_;

		template<typename T> bool EncodeT(std::istream& _in, std::ostream& _out)
		{
			size_t len1 = _in.tellg();
			BinaryReader br(_in);
			T* buf = new T[len1 / sizeof(T)];
			br.readRaw((char*)buf, len1);
			size_t len = len1 + 1;
			T currChar, prevChar;
			u8 count;


			prevChar = EOF;
			count = 0;

			currChar = *buf;
			buf++;
			len--;

			while (len)
			{
				_out << currChar;
				/* check for run */
				if (currChar == prevChar)
				{
					/* we have a run.  count run length */
					count = 0;
					currChar = *buf;
					buf++; len--;
					while (len)
					{
						if (currChar == prevChar)
						{
							count++;

							if (count == UCHAR_MAX)
							{
								/* count is as long as it can get */
								_out << currChar;
								/* force next char to be different */
								prevChar = EOF;
								break;
							}
						}
						else
						{
							/* run ended */
							_out << count;
							_out << currChar;
							prevChar = currChar;
							break;
						}

						currChar = *buf;
						buf++; len--;
					}
				}
				else
				{
					/* no run */
					prevChar = currChar;
				}

				if (!len)
				{

					_out << count;
					break;
				}
				currChar = *buf;
				buf++;
				len--;
			}
			delete[]buf;
			return true;
		}

		template<typename T>bool	DecodeT(std::istream& _in, std::ostream& _out)
		{
			size_t len = _in.tellg();
			BinaryReader br(_in);
			T* buf = new T[len / sizeof(T)];
			br.readRaw((char*)buf, len);

			T prevChar = EOF;
			T currentChar;
			while (len)
			{
				_out << *buf;
				currentChar = *buf;
				buf++;
				len--;
				/* check for run */
				if (currentChar == prevChar)
				{
					/* we have a run.  write it out. */
					s32 count = s32(*buf);
					buf++;
					len--;
					while (count > 0)
					{
						_out << currentChar;
						count--;
					}
					prevChar = EOF;     /* force next char to be different */
				}
				else
				{
					/* no run */
					prevChar = currentChar;
				}
			}
			delete[]buf;
			return true;
		}



	};
}
#endif