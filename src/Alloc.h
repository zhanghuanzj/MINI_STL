#ifndef _MINI_STL_ALLOC_H
#define _MINI_STL_ALLOC_H 

#include <cstdlib>

namespace MINI_STL
{
	class alloc
	{
	private:
		enum  { _ALIGN = 8 };
		enum  { _MAX_BYTES = 128 };
		enum  { _NFREELISTS = _MAX_BYTES/_ALIGN};
	private:
		//free-list
		union obj
		{
			union obj *next_free_list;
			char client_data[1];
		};
		static obj *free_list[_NFREELISTS];

	private:	
		//内存池分配情况
		static char *start_free;
		static char *end_free;
		static size_t heap_size;

	private:
		//上调至8的倍数
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes+_ALIGN-1)&~(_ALIGN-1));
		}
		//free-list索引
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return ((bytes+_ALIGN-1)/_ALIGN-1);
		}
		//当free-list某个槽中没有可用空间时，从内存池获取空间
		static void *refill(size_t n);
		//从内存池中获取可用空间
		static char *chunk_alloc(size_t size,int &nobjs);

	public:
		static void *allocate(size_t n);
		static void deallocate(void *p,size_t n);
		static void *reallocate(void *p,size_t old_sz,size_t new_sz);
	};

	

	
}
#endif
