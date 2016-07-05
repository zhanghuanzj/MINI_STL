#ifndef _MINI_STL_HEAP_H_
#define _MINI_STL_HEAP_H_

#include "TypeTraits.h"
namespace MINI_STL
{

	template <class RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last)
	{
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
		value_type temp = *first;
		*first = *(last-1);
		*(last-1) = temp;
		adjust_heap(first,0,last-first-1);
	}

	template <class RandomAccessIterator,class Compare>
	inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
	{
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
		value_type temp = *first;
		*first = *(last-1);
		*(last-1) = temp;
		adjust_heap(first,0,last-first-1,comp);
	}

	template <class RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first,RandomAccessIterator last)
	{
		typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
		value_type value = *(last-1);
		Distance topIndex = 0;
		Distance downIndex = last-first-1;
		Distance parent = (downIndex-1)/2;
		while(downIndex>topIndex && *(first+parent)<value)
		{
			*(first+downIndex) = *(first+parent);
			downIndex = parent;
			parent = (downIndex-1)/2;
		}
		*(first+downIndex) = value;
	}

	template <class RandomAccessIterator,class Compare>
	inline void push_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
	{
		typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
		value_type value = *(last-1);
		Distance topIndex = 0;
		Distance downIndex = last-first-1;
		Distance parent = (downIndex-1)/2;
		while(downIndex>topIndex && comp(*(first+parent),value))
		{
			*(first+downIndex) = *(first+parent);
			downIndex = parent;
			parent = (downIndex-1)/2;
		}
		*(first+downIndex) = value;
	}

	template <class RandomAccessIterator,class Distance>
	void adjust_heap(RandomAccessIterator first,Distance topIndex,Distance len)
	{
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        value_type value = *(first+topIndex);
        Distance childIndex = 2*topIndex + 2;
        while(childIndex<len)
        {
        	if (*(first+childIndex)<*(first+(childIndex-1)))
        	{
        		--childIndex;
        	}
			if ( value < *(first+childIndex))
			{
				*(first+topIndex) = *(first+childIndex);
				topIndex = childIndex;
				childIndex = 2*topIndex+2;
			}
        	else
			{
				break;
			}
        }
        if (childIndex==len&&value < *(first+childIndex-1))
        {
        	*(first+topIndex) = *(first+(childIndex-1));
        	topIndex = childIndex-1;
        }
        *(first+topIndex) = value;
	}

	template <class RandomAccessIterator,class Distance,class Compare>
	void adjust_heap(RandomAccessIterator first,Distance topIndex,Distance len,Compare comp)
	{
        typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

        value_type value = *(first+topIndex);
        Distance childIndex = 2*topIndex + 2;
        while(childIndex<len)
        {
        	if (comp( *(first+childIndex) , *(first+(childIndex-1))))
        	{
        		--childIndex;
        	}
			if (comp(value , *(first+childIndex)))
			{
				*(first+topIndex) = *(first+childIndex);
				topIndex = childIndex;
				childIndex = 2*topIndex+2;
			}
        	else
			{
				break;
			}
        }
        if (childIndex==len && comp(value,*(first+childIndex-1)))
        {
        	*(first+topIndex) = *(first+(childIndex-1));
        	topIndex = childIndex-1;
        }
        *(first+topIndex) = value;
	}

	template <class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first,RandomAccessIterator last)
	{
		while(last-first>1)
		{
			pop_heap(first,last--);
		}
	}

	template <class RandomAccessIterator,class Compare>
	void sort_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
	{
		while(last-first>1)
		{
			pop_heap(first,last--,comp);
		}
	}

	template <class RandomAccessIterator>
	inline void make_heap(RandomAccessIterator first,RandomAccessIterator last)
	{
		if (last-first<2)
		{
			return;
		}
		typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

		Distance topIndex = (last-first-2)/2;
		Distance len = last-first;
		while(topIndex>=0)
		{
			adjust_heap(first,topIndex,len);
			--topIndex;
		}
	}

	template <class RandomAccessIterator,class Compare>
	inline void make_heap(RandomAccessIterator first,RandomAccessIterator last,Compare comp)
	{
		if (last-first<2)
		{
			return;
		}
		typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
		typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;

		Distance topIndex = (last-first-2)/2;
		Distance len = last-first;
		while(topIndex>=0)
		{
			adjust_heap(first,topIndex,len,comp);
			--topIndex;
		}
	}
}


#endif