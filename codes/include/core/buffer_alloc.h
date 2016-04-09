#include <ios>
#include <cstddef>
#ifdef USE_JEMALLOC
#include "jemalloc/include/jemalloc/jemalloc.h"
#endif
namespace tng
{
	template <typename ch>
	class BufferAllocator
		/// The BufferAllocator used if no specific
		/// BufferAllocator has been specified.
	{
	public:
		typedef ch char_type;

		static char_type* allocate(std::streamsize size)
		{
#if USE_JEMALLOC
			return (char_type*)je_malloc(size);
#else
			return (char_type*)malloc(size);
#endif
		}

		static void deallocate(char_type* ptr, std::streamsize size)
		{
#if USE_JEMALLOC
			je_free (ptr);
#else
			free(ptr);
#endif
		}
	};
}
