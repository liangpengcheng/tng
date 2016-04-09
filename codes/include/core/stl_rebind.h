#ifndef __TNG_STL_REBIND_H__
#define __TNG_STL_REBIND_H__

#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <new>
#include <streambuf>
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <list>

#include "types.h"
#ifdef USE_JEMALLOC
#include "jemalloc.h"
#endif
#include "arch_detect.h"
#ifdef max
#undef max
#endif // max

namespace tng
{
template<class _Ty>
	struct _Allocator_base
	{	// base class for generic allocators
	typedef _Ty value_type;
	};

		// TEMPLATE CLASS _Allocator_base<const _Ty>
template<class _Ty>
	struct _Allocator_base<const _Ty>
	{	// base class for generic allocators for const _Ty
	typedef _Ty value_type;
	};
	template<typename T>
	class Allocator :public _Allocator_base<T>{
	public : 
		//    typedefs
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public : 
		//    convert an allocator<T> to allocator<U>
		template<typename U>
		struct rebind {
			typedef Allocator<U> other;
		};

	public : 
	Allocator()
		{	// construct default Allocator (do nothing)
		}

	Allocator(const Allocator<T>&)
		{	// construct by copying (do nothing)
		}

	template<class _Other>
		Allocator(const Allocator<_Other>&)
		{	// construct from a related Allocator (do nothing)
		}

	template<class _Other>
		Allocator<T>& operator=(const Allocator<_Other>&)
		{	// assign from a related Allocator (do nothing)
		return (*this);
		}
		//    address
		inline pointer address(reference r) { return &r; }
		inline const_pointer address(const_reference r) { return &r; }

		//    memory allocation
		inline pointer allocate(size_type cnt, 
			typename std::allocator<void>::const_pointer = 0) { 
#ifdef USE_JEMALLOC
				return reinterpret_cast<pointer>(je_malloc(cnt * sizeof (T))); 
#else
				return reinterpret_cast<pointer>(::malloc(cnt * sizeof (T))); 
#endif
			  
			}
		inline void deallocate(pointer p, size_type) { 
#ifdef USE_JEMALLOC
		  je_free(p);
#else
		  free(p);
#endif
			 
		}

		//    size
		inline size_type max_size() const { 
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

		//    construction/destruction
		inline void construct(T *_Ptr){
			::new ((void *)_Ptr) T();
		}
		inline void construct(pointer p, const T& t) { ::new ((void *)p) T(t); }
#if TNG_OS_FAMILY_WINDOWS!=1
#if __APPLE__
        template<typename U, typename... Args >
        void construct(U* p, Args&&... args) {
            new ((void*)p) U(_VSTD::forward<Args>(args)...);
        }
        
#else
		template<typename U, typename... Args >
		void construct(U* p, Args&&... args) {
			new (p) U(std::forward<Args>(args)...);
		}
#endif
#endif

		inline void destroy(pointer p) { p->~T(); }

	
		inline bool operator==( Allocator const& ) const{ return true; }
		inline bool operator!=( Allocator const& a) const{ return !operator==(a); }
	};    //    end of class Allocator 
	typedef std::basic_string<char, std::char_traits<char>, Allocator<char> >
		string;

	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator<wchar_t> >
		wstring;
	template<typename K,typename V> class unordered_map:public std::unordered_map<K,V,std::hash<K>,std::equal_to<K>,Allocator<std::pair<const K,V> > >{};

	template<typename K> class unordered_set:public std::unordered_set<K,std::hash<K>,std::equal_to<K>,Allocator<K> >{};

	template<typename T> class vector :public std::vector<T,Allocator<T> >{};

	template<typename T> class list :public std::list<T,Allocator<T> >{};

	typedef std::basic_stringbuf<char, std::char_traits<char>,
		Allocator<char> > stringbuf;
	typedef std::basic_istringstream<char, std::char_traits<char>,
		Allocator<char> > istringstream;
	typedef std::basic_ostringstream<char, std::char_traits<char>,
		Allocator<char> > ostringstream;
	typedef std::basic_stringstream<char, std::char_traits<char>,
		Allocator<char> > stringstream;


	typedef std::basic_string<sc8 , std::char_traits<char>, Allocator<char> >			a8string;
	typedef std::basic_string<wchar16, std::char_traits<wchar16>, Allocator<wchar16> >		w16string;
	typedef std::basic_string<wchar32, std::char_traits<wchar32>, Allocator<wchar32> >		w32string;
	// ------------------------------------------------------------------------------------------------
	/* magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/ */
	static const size_t InitialFNV = 2166136261U;
	static const size_t FNVMultiple = 16777619;

	/* Fowler / Noll / Vo (FNV) Hash */
	__inline size_t str_hash(const string &s)
	{
		size_t hash = InitialFNV;
		for (size_t i = 0; i < s.length(); i++)
		{
			hash = hash ^ (s[i]);       /* xor  the low 8 bits */
			hash = hash * FNVMultiple;  /* multiply by the magic number */
		}
		return hash;
	}

}
namespace std
{
	template<>
	struct hash<tng::string>
	{
		size_t operator()(const tng::string& _Keyval) const
		{	
			return (tng::str_hash(_Keyval));
		}
	};
}

#endif