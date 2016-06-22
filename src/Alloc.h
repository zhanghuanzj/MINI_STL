#ifndef _MINI_STL_ALLOC_H
#define _MINI_STL_ALLOC_H 

#include <cstdlib>
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

char *alloc::start_free = 0;
char *alloc::end_free = 0;
size_t alloc::heap_size = 0;
alloc::obj *alloc::free_list[_NFREELISTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void *alloc::allocate(size_t n)
{
	//1.大于128
	if (n>_MAX_BYTES)
	{
		return malloc(n);
	}
	int index = FREELIST_INDEX(n);
	obj *result = free_list[index];
	//2.free-list中可以分配
	if (result)
	{
		free_list[index] = result->next_free_list;
		return result;
	}
	//3.free-list相应槽中没有剩余空间，从内存池中重新获取
	else
	{
		void *r = refill(ROUND_UP(n));
		return r;
	}
}

void alloc::deallocate(void *p,size_t n)
{
	//1.大于128归还给堆
	if (n>_MAX_BYTES)
	{
		free(p);
		return;
	}
	//2.小于128归还给free-list
	else
	{
		int index = FREELIST_INDEX(n);
		obj *q = static_cast<obj*> (p);
		q->next_free_list = free_list[index];
		free_list[index] = q;
	}
}

void *alloc::reallocate(void *p,size_t old_sz,size_t new_sz)
{
	deallocate(p,old_sz);
	p = allocate(new_sz);
	return p;
}

void *alloc::refill(size_t n)
{
	int nobjs = 20;
	char *chunk = chunk_alloc(n,nobjs);
	//1.获得一个块
	if (nobjs==1)
	{
		return chunk;
	}
	//2.第一块给客端,其余用free-list串接
	obj *result = (obj*)chunk;
	obj *next_obj = (obj*)(chunk+n);
	obj *current_obj = next_obj;
	free_list[FREELIST_INDEX(n)] = current_obj;
	for (int i = 2; i <= nobjs; ++i)
	{
		if (i==nobjs)
		{
			current_obj->next_free_list = 0;
			break;
		}
		else
		{
			next_obj = (obj*)((char*)(next_obj)+n);
			current_obj->next_free_list = next_obj;
			current_obj = next_obj;
		}
	}
	return result;
}

char *alloc::chunk_alloc(size_t size,int &nobjs)
{
	char *result;
	size_t total_bytes = size*nobjs;         //请求总大小
	size_t bytes_left = end_free-start_free; //内存池中剩余空间大小

	//1.>=所需内存
	if (bytes_left>=total_bytes)
	{
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	//2.>=一个块
	else if (bytes_left>=size)
	{
		nobjs = bytes_left/size;
		total_bytes = nobjs*size;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	//3.内存池连一个块的大小都无法提供
	else
	{	
		//将剩余的内存池空间分配给free-list
		if (bytes_left>0)
		{
			int index = FREELIST_INDEX(bytes_left);
			((obj*)(start_free))->next_free_list = free_list[index];
			free_list[index] = (obj*)(start_free);
		}
		//重新为内存池申请新内存
		size_t bytes_to_get = 2*total_bytes+ROUND_UP(heap_size>>4);
		start_free = static_cast<char*>(malloc(bytes_to_get));
		//重新申请失败
		if (start_free==0)
		{
			//从free-list中查看是否有满足条件的空间
			for(int i=size;i<=_MAX_BYTES;i+=-_ALIGN)
			{
				int index = FREELIST_INDEX(i);
				if (free_list[index]!=0)
				{
					start_free = (char*)(free_list[index]);
					free_list[index] = free_list[index]->next_free_list;
					end_free = start_free+i;
					return chunk_alloc(size,nobjs);
				}
			}
			end_free = 0;
		}
		//重新申请成功
		else
		{
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size,nobjs);
		}
	}

}
#endif
