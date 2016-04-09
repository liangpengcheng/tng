#pragma once
#include "stdlib.h"
#ifdef USE_JEMALLOC
extern "C" {
#include "jemalloc/include/jemalloc/jemalloc.h"
}
#endif
namespace tng
{
	class MemObj
	{
	public:

	 	void* operator new(const size_t nSize)
	 	{
#ifdef USE_JEMALLOC
	 		void* pRet = ::je_malloc(nSize);
#else
			void* pRet = ::malloc(nSize);
#endif
			return pRet;
	 	}
		void *operator new(size_t, void *p) throw(){ return p; }
	 	void operator delete(void *p)
	 	{
	 		#ifdef USE_JEMALLOC
		  je_free(p);
#else
		  free(p);
#endif
	 	}

	 	void* operator new[](const size_t nSize)
	 	{
#ifdef USE_JEMALLOC
	 		void* pRet = ::je_malloc(nSize);
#else
			void* pRet = ::malloc(nSize);
#endif
			return pRet;
	 	}
	 	void operator delete[](void* p)
	 	{
#ifdef USE_JEMALLOC
		  je_free(p);
#else
		  free(p);
#endif
		}
		 	
	};
}