#ifndef HGL_STR_TEMPLATE_INCLUDE
#define HGL_STR_TEMPLATE_INCLUDE

#include<string.h>
#include<math.h>
namespace hgl
{

	template<typename T>
	bool islower(const T ch)
	{
		return(ch>='a'&&ch<='z');
	}


	template<typename T>
	bool isupper(const T ch)
	{
		return(ch>='A'&&ch<='Z');
	}


	template<typename T>
	bool isalpha(const T ch)
	{
		return(islower(ch)||isupper(ch));
	}


	template<typename T>
	bool isdigit(const T ch)
	{
		return(ch>='0'&&ch<='9');
	}


	template<typename T>
	bool isxdigit(const T ch)
	{
		return((ch>='0'&&ch<='9')
			||(ch>='a'&&ch<='f')
			||(ch>='A'&&ch<='F'));
	}


	template<typename T>
	bool isspace(const T ch)
	{
		return(ch==' '||ch=='\t'||ch=='\r'||ch=='\f'||ch=='\v'||ch=='\n');
	}

	template<typename T>
	bool isalnum(const T ch)
	{
		return(isalpha(ch)||isdigit(ch));
	}

	template<typename T>
	bool iscodechar(const T ch)
	{
		return(isalnum(ch)||ch=='_');
	}

	template<typename T>
	T tolower(const T ch)
	{
		if(ch>='A'&&ch<='Z')
			return ch+('a'-'A');
		else
			return ch;
	}

	template<typename T>
	T toupper(const T ch)
	{
		if(ch>='a'&&ch<='z')
			return ch+('A'-'a');
		else
			return ch;
	}


	template<typename T>
	int chricmp(T src,T dst)
	{
		return tolower(src)-tolower(dst);
	}


	template<typename T>
	int strlen(const T *str)
	{
		if(str&&*str)
		{
			const T *start=str;

			while(*str)
				str++;

			return int(str-start);
		}

		return(0);
	}


	template<typename T>
	int strlen(const T *str,int max_len)
	{
		if(str&&*str)
		{
			const T *start=str;

			do
			{
				str++;
				max_len--;

			}while(max_len>0&&*str);

			return int(str-start);
		}

		return(0);
	}


	template<typename T>
	void strcpy(T *dst,const T *src)
	{
		if(!dst)return;

		if(!src||!(*src))
			*dst=0;
		else
		{
			while(*src)
			{
				*dst=*src;
				dst++;
				src++;
			}

			*dst=0;
		}
	}


	template<typename T>
	void strcpy(T *dst,int count,const T *src)
	{
		if(!dst)return;

		if(!src||!(*src)||count<=0)
		{

			*dst=0;
			return;
		}

		if(!src||!(*src))
			*dst=0;
		else
		{
			while(*src&&count)
			{
				*dst=*src;
				dst++;
				src++;
				count--;
			}

			if(count)
				*dst=0;
		}
	}

	template<typename T>
	void strcpy(T *dst,const T *src,int count)
	{
		if(!dst)return;

		if(!src||!(*src)||count<=0)
		{
			*dst=0;
			return;
		}

		while(*src&&count)
		{
			*dst=*src;
			dst++;
			src++;
			count--;
		}

		if(count)
			*dst=0;
	}


	template<typename T>
	T *strstr(T *str1,T *str2)
	{
		T *cp = (T *) str1;
		T *s1, *s2;

		if ( !*str2)
			return (T *)str1;

		while (*cp)
		{
			s1 = cp;
			s2 = (T *) str2;

			while ( *s1 && *s2 && !(*s1-*s2) )
				s1++, s2++;

			if (!*s2)
				return(cp);

			cp++;
		}

		return(0);
	}


	template<typename T>
	void strcat(T *dst,const T ch)
	{
		if(!dst||!ch)return;

		while(*dst)
			dst++;			

		*dst++=ch;
		*dst=0;
	}


	template<typename T>
	void strcat(T *dst,const T *src)
	{
		if(!dst||!src||!(*src))return;

		while(*dst)
			dst++;			

		while(*src)
		{
			*dst=*src;
			dst++;
			src++;
		}

		*dst=0;
	}


	template<typename T>
	void strcat(T *dst,const T *src,int count)
	{
		if(!dst||!src||!(*src)||count<=0)return;

		while(*dst)
			dst++;          

		while(*src&&count)
		{
			*dst=*src;
			dst++;
			src++;
			count--;
		}

		if(count)
			*dst=0;
	}


	template<typename TS,typename TC>
	TS *strchr(TS *str,TC ch)
	{
		if(!str||!(*str))return(0);

		while(*str)
		{
			if(*str==ch)
				return(str);
			else
				str++;
		}

		return(0);
	}


	template<typename TS,typename TC>
	TS *strchr(TS *str,TC ch,int n)
	{
		if(!str||!(*str))return(0);

		while(*str&&n--)
		{
			if(*str==ch)
				return(str);
			else
				str++;
		}

		return(0);
	}

	template<typename T>
	T *strrchr(T *str,T ch)
	{
		if(!str||!(*str))return(0);

		T *result=0;

		while(*str)
		{
			if(*str==ch)result=(T *)str;

			str++;
		}

		return(result);
	}


	template<typename S,typename D>
	int strcmp(S *src,D *dst)
	{
		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		while((*src)&&(*dst)&&(*src==*dst))
		{
			src++;
			dst++;
		}

		return(*src-*dst);
	}


	template<typename S,typename D>
	int strcmp(S *src,int src_size,D *dst,int dst_size)
	{
		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		if(src_size<=0)
		{
			if(dst_size<=0)return(0);
						else return(-1);
		}

		if(dst_size<=0)return(1);

		while((src_size)&&(dst_size)&&(*src==*dst))
		{
			src++;
			dst++;

			src_size--;
			dst_size--;
		}

		if(src_size)
		{
			if(dst_size)
				return(*src-*dst);
			else
				return 1;
		}

		if(dst_size)
			return -1;
		else
			return 0;
	}


	template<typename S,typename D>
	int strcmp(S *src,D *dst,int count)
	{
		if(count<=0)return(0);

		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		while(count&&(*src)&&(*dst)&&(*src==*dst))
		{
			src++;
			dst++;
			count--;
		}

		if(count==0)
			return(0);

		return(*src-*dst);
	}


	template<typename S,typename D>
	int stricmp(S *src,D *dst)
	{
		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		int gap;

		while(*src&&*dst)
		{
			gap=chricmp(*src,*dst);

			if(gap)
				return gap;

			src++;
			dst++;
		}

		return(*src-*dst);
	}


	template<typename S,typename D>
	int stricmp(S *src,int src_size,D *dst,int dst_size)
	{
		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		if(src_size<=0)
		{
			if(dst_size<=0)return(0);
						else return(-1);
		}

		if(dst_size<=0)return(1);

		int gap;

		while(src_size&&dst_size)
		{
			gap=chricmp(*src,*dst);

			if(gap)
				return gap;

			src++;
			dst++;
			src_size--;
			dst_size--;
		}

		if(src_size)
		{
			if(dst_size)
				return chricmp(*src,*dst);
			else
				return 1;
		}

		if(dst_size)
			return -1;
		else
			return 0;
	}


	template<typename S,typename D>
	int stricmp(S *src,D *dst,int count)
	{
		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		int gap;

		while((count)&&(*src)&&(*dst))
		{
			gap=chricmp(*src,*dst);

			if(gap)
				return gap;

			src++;
			dst++;
			count--;
		}

		if(count==0)
			return(0);

		return(*src-*dst);
	}


	template<typename S,typename D>
	int charset_cmp(S *src,D *dst)
	{
		if(!src)
		{
			if(!dst)return(0);
				else return(-1);
		}

		if(!dst)return(1);

		int gap;

		while(*src&&*dst)
		{
			if(!isalnum(*src)){src++;continue;}
			if(!isalnum(*dst)){dst++;continue;}

			gap=chricmp(*src,*dst);

			if(gap)
				return gap;

			src++;
			dst++;
		}

		return(*src-*dst);
	}


	template<typename T>
	T *trimleft(T *src,int &len)
	{
		const T *p=src;

		while(*p&&isspace(*p)&&len)
		{
			p++;
			len--;
		}

		if(len<=0)
			return(0);

		T *new_str=new T[len+1];

		memcpy(new_str,p,len*sizeof(T));
		new_str[len]=0;
		return new_str;
	}

	template<typename T>
	T *trimright(T *src,int &len)
	{
		const T *p=src+len-1;

		while(isspace(*p)&&len)
		{
			p--;
			len--;
		}

		if(len<=0)
			return(0);

		T *new_str=new T[len+1];

		memcpy(new_str,src,len*sizeof(T));
		new_str[len]=0;
		return new_str;
	}


	template<typename T>
	T *trim(T *src,int &len)
	{
		const T *sp=src;
		const T *ep=src+len-1;

		while(*sp&&isspace(*sp)&&len)
		{
			sp++;
			len--;
		}

		while(isspace(*ep)&&len)
		{
			ep--;
			len--;
		}

		if(len<=0)
			return(0);

		T *new_str=new T[len+1];

		memcpy(new_str,sp,len*sizeof(T));
		new_str[len]=0;
		return new_str;
	}


	template<typename T>
	void replace_extname(const T *old_filename,T *new_filename,const T *new_extname)
	{
		T *p=strrchr(old_filename,L'.');

		if(p)
		{
			strcpy(new_filename,old_filename,p-old_filename+1);
			strcpy(new_filename+(p-old_filename+1),new_extname);
		}
		else
		{
			const int l=strlen(old_filename);

			strcpy(new_filename,old_filename,l);
			new_filename[l]=L'.';
			strcpy(new_filename+l+1,new_extname);
		}
	}

	template<typename T>
	void replace_extname(T *filename,const T *new_extname)
	{
		T *p=strrchr(filename,L'.');

		if(p)
		{
			strcpy(p+1,new_extname);
		}
		else
		{
			strcat(filename,L'.');
			strcat(filename,new_extname);
		}
	}


	template<typename T>
	T *create_copy(const T *str,int size=-1)
	{
		if(!str)return(0);

		T *copy;

		if(size==-1)size=strlen(str);

		if(size==0)return(0);

		size++;

		copy=new T[size];
		strcpy(copy,str,size);

		return(copy);
	}


	template<typename T>
	int replace(T *str,const T tch,const T sch)
	{
		if(!str)
			return(0);

		int count=0;

		while(*str)
		{
			if(*str==sch)
			{
				*str=tch;
				count++;
			}

			str++;
		}

		return(count);
	}

	template<typename T>
	void toupper(T *str)
	{
		if(!str)return;

		while(*str)
		{
			if(*str>='a'&&*str<='z')
				*str-=32;

			str++;
		}
	}



	template<typename T>
	void tolower(T *str)
	{
		if(!str)return;

		while(*str)
		{
			if(*str>='A'&&*str<='Z')
				*str+=32;

			str++;
		}
	}


	template<typename T>
	void toupper(const T *src,T *dst)
	{
		if(!dst)return;

		if(!src)
		{
			*dst=0;
			return;
		}

		do
		{
			if(*src>='A'&&*src<='Z')
				*dst++=*src-32;
			else
				*dst++=*src;

			src++;
		}while(*src);

		*dst=0;
	}


	template<typename T>
	void tolower(const T *src,T *dst)
	{
		if(!dst)return;

		if(!src)
		{
			*dst=0;
			return;
		}

		do
		{
			if(*src>='A'&&*src<='Z')
				*dst++=*src+32;
			else
				*dst++=*src;

			src++;
		}while(*src);

		*dst=0;
	}


	template<typename T>
	int stat_char(T *str,T ch)
	{
		if(!str)return(0);

		int count=0;

		while(*str)
		{
			if(*str==ch)
				count++;

			str++;
		}

		return(count);
	}


	template<typename T>
	int stat_line(T *str)
	{
		if(!str)return(0);

		T *p=str+strlen(str)-1;

		if(*p=='\n')return(stat_char(str,'\n'));
			else return(stat_char(str,'\n')+1);
	}

	template<typename R,typename S>
	bool stoi(S *str,R &result)
	{
		if(!str)
		{
			result=0;
			return(false);
		}

		result=0;
		bool zf=true;

		if(*str=='+')
			str++;

		if(*str=='-')
		{
			zf=false;
			str++;
		}

		while(*str&&*str>='0'&&*str<='9')
		{
			result*=10;
			result+=(*str-'0');

			str++;
		}

		if(!zf)
			result=-result;

		return(true);
	}

	template<typename R,typename S>
	bool stou(S *str,R &result)
	{
		if(!str)
		{
			result=0;
			return(false);
		}

		result=0;

		while(*str)
		{
			if(*str>='0'&&*str<='9')
			{
				result*=10;
				result+=(*str-'0');

				str++;
			}
			else
				break;
		}

		return(true);
	}

	template<typename R,typename S>
	bool stof(S *str,R &result)
	{
		if(!str)
		{
			result=0;
			return(false);
		}

		bool zf=true;

		result=0;

		if(*str=='+')
			str++;

		if(*str=='-')
		{
			zf=false;
			str++;
		}

		while(*str&&*str>='0'&&*str<='9')
		{
			result*=10;
			result+=(*str-'0');

			str++;
		}

		if(*str!='.')
		{
			if(!zf)
				result=-result;

			return(true);
		}

		str++;	

		R pos=0.1f;

		while(*str&&*str>='0'&&*str<='9')
		{
			result+=R(*str-'0')*pos;

			pos*=0.1f;

			str++;
		}

		if(!zf)
			result=-result;

		return(true);
	}

	template<typename R,typename S>
	bool etof(S *str,R &result)
	{
		double temp;

		if(!stof(str,temp))
			return(false);

		while(*str!='e'&&*str!='E')
		{
			if(!*str)
			{
				result=R(temp);
				return(true);
			}

			str++;
		}

		if(*str=='e'||*str=='E')
		{
			double power;

			if(stof(str+1,power))
			{
				result=R(temp*pow(10,power));

				return(true);
			}
		}

		result=R(temp);
		return(false);
	}


	template<typename T>
	bool stob(T *str,bool &value)
	{
		if(!str)
		{
			value=false;
			return(false);
		}

		if(*str=='T'||*str=='t'			//true/false
		||*str=='Y'||*str=='y'			//yes/no
		||*str=='M'||*str=='m'			//male/women
		||*str=='1')					//1/0
		{
			value=true;
			return(true);
		}
		else
		{
			value=false;
			return(false);
		}
	}


	template<typename T,typename I>
	T *itos(T *str,int size,const I num)
	{
		if(!str||size<=0)return(NULL);

		T *p=str;
		T buf[sizeof(I)*8],*bp;

		I value=num;

		if(value<0)
		{
			*p++='-';
			size--;

			value=-value;
		}

		bp=buf;

		while(true)
		{
			*bp++=(value%10)+'0';

			if ((value = value / 10) == 0)
				break;
		}

		while(bp--!=buf&&size--)
			*p++=*bp;

		if(size)
			*p=0;

		return(str);
	}


	template<typename T,typename U>
	T *utos(T *str,int size,U value)
	{
		if(!str||size<=0)return(NULL);

		T *p=str;
		T buf[sizeof(U)*8],*bp;

		bp=buf;

		while(true)
		{
			*bp++=(value%10)+'0';

			if ((value = value / 10) == 0)
				break;
		}

		while(bp--!=buf&&size--)
			*p++=*bp;

		if(size)
			*p=0;

		return(str);
	}


	template<typename T,typename U>
	T *htos(T *str,int size,U value,bool upper=true)
	{
		if(!str||size<=0)return(NULL);

		const T sc=(upper?'A':'a')-10;
		int h,l;

		T buf[(sizeof(U)+1)<<1];
		T *bp=buf;
		T *p=str;

		while(value>0)
		{
			l=value&0xF;

			if(l>=10)
				*bp++=l+sc;
			else
				*bp++=l+'0';

			value>>=4;
			h=value&0xF;

			if(h>=10)
				*bp++=h+sc;
			else
				*bp++=h+'0';

			value>>=4;
		}

		while(bp--!=buf&&size--)
			*p++=*bp;

		if(size)
			*p=0;

		return(str);
	}


	template<typename T,typename F>
	T *ftos(T *str,int size,int fsize,F value)
	{
		const long integer=(long)value;		

		T *p=str;

		if(integer==0&&value<0)				
			*p++='-';

		itos(p,size,integer);

		int len=strlen(str);

		if(len>=size)
			return(str);

		p=str+len;

		value-=integer;						

		if(value<0)
			value=-value;

		const float min_value=pow(0.1,fsize);

		if(value<min_value)
		{
			*p=0;
			return(str);
		}

		*p++=L'.';						
		len++;

		while(value>min_value&&len<size&&fsize--)
		{
			value*=10;						
			*p++='0'+int(value);

			value-=int(value);				
			len++;
		}

		*p=0;
		return(str);
	}

	template<typename T,typename F>
	T *ftos(T *str,int size,F value)
	{
		return ftos(str,size,4,value);
	}


	template<typename T,typename V>
	bool int_to_size(T *str,int size,V value)
	{
		const T name[]={'K','M','G','T','P','E','Z','Y'};

		double new_value=value;
		int pos=0;

		while(new_value>=1024&&size--)
		{
			pos++;
			new_value/=1024;

			if(name[pos]==0)
				return(false);	
		}

		const float f=new_value;

		ftos(str,size,2,f);

		T *p=str+strlen(str);

		if(pos)
			*p++=name[pos-1];

		*p++='B';
		*p=0;

		return(true);
	}

	template<typename T>
	int find_str_array(int count,const T **str_array,const T *str)
	{
		for(int i=1;i<count;i++)
			if(stricmp<T>(str_array[i],str)==0)return(i);

		return(-1);
	}


	template<typename T,typename I>
	int parse_int_array(const T *str,I *result,int max_count,const T end_char=0,const T **end_pointer=0)
	{
		if(!str||!result||max_count<=0)return(-1);

		const T *p,*sp;
		int count=0;

		sp=str;
		p=sp;

		while(*p&&*p!=end_char)
		{
			if(hgl::isdigit(*p)||*p=='-')
			{
				p++;
				continue;
			}

			if(hgl::stoi(sp,*result))
			{
				count++;
				max_count--;

				if(max_count<=0)
				{
					if(end_pointer)
						*end_pointer=p;

					return(count);
				}

				result++;

				p++;
				sp=p;

				continue;
			}

			if(end_pointer)
				*end_pointer=p;

			return count;
		}

		if(p>sp)
		{
			stoi(sp,*result);
			count++;
		}

		if(end_pointer)
			*end_pointer=p;
		return(count);
	}


	template<typename T,typename I>
	int parse_float_array(const T *str,I *result,int max_count,const T end_char=0,const T **end_pointer=0)
	{
		if(!str||!result||max_count<=0)return(-1);

		const T *p,*sp;
		int count=0;

		sp=str;
		p=sp;

		while(*p&&*p!=end_char)
		{
			if(hgl::isdigit(*p)||*p=='-'||*p=='.')
			{
				p++;
				continue;
			}

			if(hgl::stof(sp,*result))
			{
				count++;
				max_count--;

				if(max_count<=0)
				{
					if(end_pointer)
						*end_pointer=p;

					return(count);
				}

				result++;

				p++;
				sp=p;

				continue;
			}

			if(end_pointer)
				*end_pointer=p;

			return count;
		}

		if(p>sp)
		{
			stof(sp,*result);
			count++;
		}

		if(end_pointer)
			*end_pointer=p;
		return(count);
	}


	template<typename T> int string_to_enum(const T **list,const T *str)
	{
		if(!str||!list)return(-1);

		int index=0;

		do
		{
			if(stricmp(list[index],str)==0)
				return index;

			index++;
		}while(*list[index]);

		return(-1);
	}


	template<typename T> bool check_codestr(const T *str)
	{
		if(!str)return(false);

		if((!isalpha(*str))&&(*str!='_'))		
			return(false);

		str++;

		if(!(*str))								
			return(false);

		while(*str)
			if(!iscodechar(*str++))
				return(false);

		return(true);
	}

	template<typename T> bool check_error_char(const T *str)
	{
		if(!str)return(false);

		while(*str)
		{
			if(isspace(*str))
				return(false);

			if(*str==' '
			||*str=='/'
			||*str=='\\'
			||*str=='|'
			||*str=='?'
			||*str=='%'
			||*str=='"'
			||*str=='\''
			||*str=='*'
			)
				return(false);

			str++;
		}

		return(true);
	}
}//namespace hgl
#endif//HGL_STR_TEMPLATE_INCLUDE
