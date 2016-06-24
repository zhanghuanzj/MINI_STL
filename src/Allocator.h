#ifndef _MINI_STL_ALLOCATOR_H_
#define _MINI_STL_ALLOCATOR_H_ 

#include "Alloc.h"

namespace MINI_STL
{
	template<class T>
	class Allocator
	{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	public:
		static T *allocate();                               //分配一个T大小的内存
		static T *allocate(size_t n);						//分配n个T大小的内存
		static void deallocate(T *p);						//回收一个T大小的内存
		static void deallocate(T *p, size_t n);				//回收n个T大小的内存
	};

	template<class T>
	T *Allocator<T>::allocate(){
		return static_cast<T *>(alloc::allocate(sizeof(T)));
	}

	template<class T>
	T *Allocator<T>::allocate(size_t n){
		if (n == 0) return 0;
		return static_cast<T *>(alloc::allocate(sizeof(T) * n));
	}

	template<class T>
	void Allocator<T>::deallocate(T *p){
		alloc::deallocate(static_cast<void *>(p), sizeof(T));
	}

	template<class T>
	void Allocator<T>::deallocate(T *p, size_t n){
		if (n == 0) return;
		alloc::deallocate(static_cast<void *>(p), sizeof(T)* n);
	}
}

#endif