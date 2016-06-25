#ifndef _MINI_STL_UNINITIALIZED_H_
#define _MINI_STL_UNINITIALIZED_H_

#include "Algorithm.h"
#include "TypeTraits.h"
#include "Iterator.h"
#include "Construct.h"

namespace MINI_STL
{
	/************************************************************************/
	/*	将[first,last)区间内的数据copy到dest开始的内存中
		1.根据数据类型而做不同的拷贝策略
		2.并添加const char*和const wchar_t*两种特化版本
	*/
	/************************************************************************/
	template<class InputIterator,class ForwardIterator>
	ForwardIterator _Uninitialized_copy_aux(InputIterator first, InputIterator last,ForwardIterator dest, _true_type)
	{
		return copy(first,last,dest);
	}

	template<class InputIterator,class ForwardIterator>
	ForwardIterator _Uninitialized_copy_aux(InputIterator first, InputIterator last,ForwardIterator dest, _false_type)
	{
		ForwardIterator cur = dest;
		for(;first!=last;++first,++cur)
		{
			construct(&*cur,*first);
		}
		return cur;
	}
	//将[first,last)区间内的数据copy到dest开始的内存中,返回尾后迭代器
	template<class InputIterator,class ForwardIterator>
	ForwardIterator Uninitialized_copy(InputIterator first, InputIterator last,ForwardIterator dest)
	{
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _Uninitialized_copy_aux(first,last,dest,isPODType());
	}
	//将[first,last)区间内的数据copy到dest开始的内存中,返回尾后迭代器
	inline char* Uninitialized_copy(const char *first,const char *last,char *dest)
	{
		memmove(dest,first,last-first);
		return dest+(last-first);
	}
	//将[first,last)区间内的数据copy到dest开始的内存中,返回尾后迭代器
	inline wchar_t* Uninitialized_copy(const wchar_t *first,const wchar_t *last,wchar_t *dest)
	{
		memmove(dest,first,sizeof(wchar_t)*(last-first));
		return dest+(last-first);
	}

	/************************************************************************/
	/*	将[first,last)区间内的数据用T填充
		1.根据数据类型而做不同的填充策略
	*/
	/************************************************************************/
	template<class ForwardIterator, class T>
	void _Uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,const T& value, _true_type)
	{
		fill(first, last, value);
	}

	template<class ForwardIterator, class T>
	void _Uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,const T& value, _false_type)
	{
		for (; first != last; ++first){
			construct(first, value);
		}
	}
	//将[first,last)区间内的数据用T填充
	template<class ForwardIterator, class T>
	void Uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_Uninitialized_fill_aux(first, last, value, isPODType());
	}

	/************************************************************************/
	/*	从first开始的n个数据块用x来填充
		1.根据数据类型而做不同的填充策略
	*/
	/************************************************************************/
	template<class ForwardIterator, class Size, class T>
	ForwardIterator _Uninitialized_fill_n_aux(ForwardIterator first,Size n, const T& x, _true_type)
	{
		return fill_n(first, n, x);
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _Uninitialized_fill_n_aux(ForwardIterator first,Size n, const T& x, _false_type)
	{
		int i = 0;
		for (; i != n; ++i){
			construct((T*)(first + i), x);
		}
		return (first + i);
	}
	//从first开始的n个数据块用x来填充
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator Uninitialized_fill_n(ForwardIterator first,Size n, const T& x)
	{
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _Uninitialized_fill_n_aux(first, n, x, isPODType());
	}
}

#endif