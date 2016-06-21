#ifndef _MINI_STL_CONSTRUCT_H
#define _MINI_STL_CONSTRUCT_H

#include <new>

//构造函数,使用placement new构造
template <class T1,class T2>
inline void construct(T1 *p,const T2 &value)
{
	new(p) T1(value);
}

//析构函数,调用类型的析构函数
template <class T>
inline void destroy(T *pointer)
{
	pointer->~T();
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first,ForwardIterator last)
{
	__destroy(first,last,value_type(first));
}

template <class ForwardIterator>
inline void __destroy(ForwardIterator first,ForwardIterator last,T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destroy_aux(first,last,trivial_destructor());
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__false_type)
{
	while(first<last)
	{
		destroy(&*(first++));
	}
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first,ForwardIterator last,__true_type)
{
	/*do nothing*/
}

#endif

