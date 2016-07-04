#ifndef _MINI_STL_DEQUE_H_
#define _MINI_STL_DEQUE_H_

#include "Algorithm.h"
#include "Construct.h"
#include "Allocator.h"
#include "TypeTraits.h"
#include "Iterator.h"
#include "Uninitialized.h"

namespace MINI_STL
{
	inline size_t deque_buf_size(size_t size)
	{
		return size<512?size_t(512/size):size_t(1);
	}

	template<class T,class Ref,class Ptr>
	struct deque_iterator
	{
		typedef deque_iterator<T,T&,T*>     			iterator;
		typedef deque_iterator<T,const T&,const T*>  	const_iterator;
		static size_t buffer_size(){return deque_buf_size(sizeof(T));}

		typedef random_access_iterator_tag iterator_category;
		typedef T 	value_type;
		typedef T*  pointer;
		typedef T&  reference;
		typedef size_t size_type;
		typedef ptrdiff_t	difference_type;
		typedef T** map_pointer;

		T*	M_cur;
		T*  M_first;
		T*  M_last;
		map_pointer M_node;

		deque_iterator(T* x,map_pointer y):M_cur(x),M_first(*y),M_last(*y+deque_buf_size()),M_node(y){}
		deque_iterator():M_cur(0),M_first(0),M_last(0),M_node(0){}
		deque_iterator(const iterator& x):M_cur(x.M_cur),M_first(x.M_first),M_last(x.M_last),M_node(x.M_node){}

		reference operator*(){return *M_cur;}
		pointer operator->(){return M_cur;}

		difference_type operator-(const deque_iterator& x)const
		{
			return difference_type(buffer_size())*(M_node-x.M_node-1)+(M_cur- M_first)+(x.M_last-x.M_cur);
		}

		void set_node(map_pointer new_node)
		{
			M_node = new_node;
			M_first = *new_node;
			M_last = M_first + difference_type(buffer_size());
		}

		deque_iterator& operator++()
		{
			++M_cur;
			if (M_cur==M_last)
			{
				set_node(M_node+1);
				M_cur = M_first;
			}
			return *this;
		}

		deque_iterator operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		deque_iterator& operator--()
		{
			if (M_cur==M_first)
			{
				set_node(M_node-1);
				M_cur = M_last;
			}
			--M_cur;
			return *this;
		}

		deque_iterator operator--(int)
		{
			auto temp = *this;
			--*this;
			return temp;
		}

		deque_iterator& operator+=(difference_type n)
		{
			difference_type offset = n + M_cur - M_first;
			if (offset >= 0 && offset < difference_type(buffer_size()))
			{
				M_cur += n;
			}
			else 
			{
				difference_type node_offset = offset>0? offset/difference_type(buffer_size()):-difference_type((-offset-1)/buffer_size())-1;
				set_node(M_node + node_offset);
				M_cur = M_first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}

		deque_iterator operator+(difference_type n)const
		{
			auto temp = *this;
			return temp += n;
		}

		deque_iterator& operator-=(difference_type n) {return *this += -n;}

		deque_iterator operator-(difference_type n)const
		{
			auto temp = *this;
			return temp -= n;
		}

		reference operator[](difference_type n)const {return *(*this + n);}

		bool operator==(const deque_iterator& x)const {return M_cur == x.M_cur;}
		bool operator!=(const deque_iterator& x)const {return M_cur != x.M_cur;}
		bool operator<(const deque_iterator& x)const
		{
			return (M_node == x.M_node) ? (M_cur<x.M_cur) : (M_node < x.M_node);
		}
		bool operator>(const deque_iterator& x)const {return x<*this;}
		bool operator<=(const deque_iterator& x)const {return !(*this>x);}
		bool operator>=(const deque_iterator& x)const {return !(*this<x);}
	};

	template<class T,class Ref,class Ptr>
	inline deque_iterator<T,Ref,Ptr>
	operator+(ptrdiff_t n,const deque_iterator<T,Ref,Ptr>& x)
	{
		return x+n;
	}


	template <class T>
	class deque
	{
	public:
		typedef T 	value_type;
		typedef T* 	pointer;
		typedef const T* const_pointer;
		typedef T& 	reference;
		typedef const T& const_reference;
		typedef size_t 	size_type;
		typedef ptrdiff_t 	difference_type;

		typedef deque_iterator<T,T&,T*>		iterator;
		typedef deque_iterator<T,const T&,const T*>	const_iterator;


	protected:
		typedef pointer*	map_pointer;
		typedef Allocator<T> dataAllocator;
		typedef Allocator<T*> mapAllocator;
		static size_t buffer_size() { return deque_buf_size(sizeof(T)); }
	protected:
		T* allocate_nodes()
		{
			return dataAllocator::allocate(deque_buf_size(sizeof(T)));
		}
		void deallocate_nodes(T* p)
		{
			dataAllocator::deallocate(p,deque_buf_size(sizeof(T)));
		}
		T** allocate_map(size_t n) {return mapAllocator::allocate(n);}
		void deallocate_map(T** p,size_t n) {mapAllocator::deallocate(p,n);}

	protected:
		void fill_initialize(const value_type& value);
		void create_map_and_nodes(size_type num_elements);
		enum { initial_map_size = 8 };

		template<class Integer>
		void deque_aux(Integer n,Integer val,_true_type);
		template<class InputIterator>
		void deque_aux(InputIterator first,InputIterator last,_false_type);
	public:
		template<class InputIterator>
		void range_initialize(InputIterator first,InputIterator last,input_iterator_tag);
		template<class ForwardIterator>
		void range_initialize(ForwardIterator first,ForwardIterator last,forward_iterator_tag);
		//Constructor
		deque():start(),finish(),map(0),map_size(0){ create_map_and_nodes(0); }
		explicit deque(size_type n):start(),finish(),map(0),map_size(0){create_map_and_nodes(n); fill_initialize(T());}
		deque(size_type n,const T& value):start(),finish(),map(0),map_size(0){create_map_and_nodes(n); fill_initialize(value);}
		deque(const deque& x)
		{	
			create_map_and_nodes(x.size());  
			Uninitialized_copy(x.begin(),x.end(),start);
		}
		template<class InputIterator>
		deque(InputIterator first,InputIterator last);
		~deque(){
			clear(); 
			dataAllocator::deallocate(start.M_first,buffer_size()); 
			mapAllocator::deallocate(map,map_size);}

		iterator begin(){return start;}
		iterator end(){return finish;}
		const_iterator begin()const {return start;}
		const_iterator end()const {return finish;}

		reference operator[](size_type n){return start[difference_type(n)];}
		reference front(){return *start;}
		reference back(){return *(finish-1);}

		size_type size() const{return finish-start;}
		size_type max_size()const{return size_type(-1);}
		bool empty()const{return finish==start;}

	protected:
		void reallocate_map(size_type nodes_to_add,bool add_at_front);
		void reserve_map_at_back(size_type nodes_to_add=1);
		void reserve_map_at_front(size_type nodes_to_add=1);
		void push_back_aux(const value_type& v);
		void push_front_aux(const value_type& v);
		void pop_back_aux();
		void pop_front_aux();
		iterator insert_aux(iterator position,const value_type& x);
	public:
		void push_back(const value_type& v);
		void push_front(const value_type& v);
		void pop_back();
		void pop_front();
		void clear();
		iterator erase(iterator position);
		iterator erase(iterator first,iterator last);
		iterator insert(iterator position,const value_type& x);

		void swap(deque& x);
		deque& operator=(const deque& x);

	public:
		iterator start;
		iterator finish;

	private:
		map_pointer map;
		size_type map_size;


	};

	template<class T>
	template<class InputIterator>
	void deque<T>::range_initialize(InputIterator first,InputIterator last,input_iterator_tag)
	{
		create_map_and_nodes(0);
		for(;first!=last;++first)
			push_back(*first);
	}

	template<class T>
	template<class ForwardIterator>
	void deque<T>::range_initialize(ForwardIterator first,ForwardIterator last,forward_iterator_tag)
	{
		size_type n = distance(first,last);
		create_map_and_nodes(n);
		map_pointer cur;
		for(cur=start.M_node;cur<finish.M_node;++cur)
		{
			ForwardIterator mid = first;
			advance(mid,buffer_size());
			Uninitialized_copy(first,mid,*cur);
			first = mid;
		}
		Uninitialized_copy(first,last,finish.M_first);
	}

	template<class T>
	template<class Integer>
	void deque<T>::deque_aux(Integer n,Integer val,_true_type)
	{
		create_map_and_nodes(n);
		fill_initialize(val);
	}

	template<class T>
	template<class InputIterator>
	void deque<T>::deque_aux(InputIterator first,InputIterator last,_false_type)
	{
		range_initialize(first,last,iterator_category(first));
	}

	template<class T>
	void deque<T>::fill_initialize(const value_type& value)
	{
		map_pointer cur;
		for(cur=start.M_node;cur<finish.M_node;++cur)
		{
			Uninitialized_fill(*cur,*cur + buffer_size(),value);
		}
		Uninitialized_fill(finish.M_first,finish.M_cur,value);
	}

	template<class T>
	void deque<T>::create_map_and_nodes(size_type num_elements)
	{
		size_type num_nodes = num_elements/buffer_size()+1;
		map_size = max((size_type)initial_map_size,num_nodes+2);
		map = mapAllocator::allocate(map_size);

		map_pointer nstart = map + (map_size - num_nodes)/2;
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur;

		for(cur=nstart;cur<=nfinish;++cur)
		{
			*cur = allocate_nodes();
		}

		start.set_node(nstart);
		start.M_cur = start.M_first;
		finish.set_node(nfinish);
		finish.M_cur = finish.M_first + num_elements%buffer_size();
	}

	template<class T>
	template<class InputIterator>
	deque<T>::deque(InputIterator first,InputIterator last)
	{
		typedef typename _is_Integer<InputIterator>::_Integral _Integral;
		deque_aux(first,last,_Integral());
	}

	template<class T>
	void deque<T>::reallocate_map(size_type nodes_to_add,bool add_at_front)
	{
		size_type old_num_nodes = finish.M_node - start.M_node + 1;
		size_type new_num_nodes = old_num_nodes+nodes_to_add;

		map_pointer new_start;
		if (map_size>2*new_num_nodes)  //原来的空间够，则移动
		{
			new_start = map + (map_size - new_num_nodes)/2+(add_at_front?nodes_to_add:0);
			if (new_start < start.M_node)  //往左移
			{
				MINI_STL::copy(start.M_node,finish.M_node+1,new_start);
			}
			else  //往右移
			{
				MINI_STL::copy_backward(start.M_node,finish.M_node+1,new_start+old_num_nodes);
			}
		}
		else  //重新分配map
		{
			size_type new_map_size = map_size + max(map_size,nodes_to_add) + 2;
			map_pointer new_map = mapAllocator::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_num_nodes)/2 + (add_at_front?nodes_to_add:0);
			MINI_STL::copy(start.M_node,finish.M_node+1,new_start);
			mapAllocator::deallocate(map,map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_start);
		finish.set_node(new_start+old_num_nodes-1);
	}

	template<class T>
	void deque<T>::reserve_map_at_back(size_type nodes_to_add)
	{
		if (nodes_to_add+1>map_size-(finish.M_node-map))
		{
			reallocate_map(nodes_to_add,false);
		}
	}

	template<class T>
	void deque<T>::reserve_map_at_front(size_type nodes_to_add)
	{
		if (nodes_to_add+1>(size_type)(start.M_node-map))
		{
			reallocate_map(nodes_to_add,true);
		}
	}

	template<class T>
	void deque<T>::push_back_aux(const value_type& v)
	{
		reserve_map_at_back();
		*(finish.M_node+1) = allocate_nodes();
		construct(finish.M_cur,v);
		finish.set_node(finish.M_node+1);
		finish.M_cur = finish.M_first;
	}

	template<class T>
	void deque<T>::push_front_aux(const value_type& v)
	{
		reserve_map_at_front();
		*(start.M_node-1) = allocate_nodes();
		start.set_node(start.M_node-1);
		start.M_cur = start.M_last-1;
		construct(start.M_cur,v);
	}

	template<class T>
	void deque<T>::push_back(const value_type& v)
	{
		if (finish.M_cur!=finish.M_last-1)
		{
			construct(finish.M_cur,v);
			++finish.M_cur;
		}
		else
		{
			push_back_aux(v);
		}
	}

	template<class T>
	void deque<T>::push_front(const value_type& v)
	{
		if (start.M_cur!=start.M_first)
		{
			construct(start.M_cur-1,v);
			--start.M_cur;
		}
		else
		{
			push_front_aux(v);
		}
	}

	template<class T>
	void deque<T>::pop_back_aux()
	{
		deallocate_nodes(finish.M_first);
		finish.set_node(finish.M_node-1);
		finish.M_cur = finish.M_last-1;
		destroy(finish.M_cur);
	}

	template<class T>
	void deque<T>::pop_back()
	{
		if (finish.M_cur != finish.M_first)
		{
			--finish.M_cur;
			destroy(finish.M_cur);
		}
		else
		{
			pop_back_aux();
		}
	}

	template<class T>
	void deque<T>::pop_front_aux()
	{
		destroy(start.M_cur);
		deallocate_nodes(start.M_first);
		start.set_node(start.M_node+1);
		start.M_cur = start.M_first;
	}

	template<class T>
	void deque<T>::pop_front()
	{
		if (start.M_cur!=start.M_last-1)
		{
			destroy(start.M_cur);
			++start.M_cur;
		}
		else
		{
			pop_front_aux();
		}
	}

	template<class T>
	void deque<T>::clear()
	{
		for(map_pointer node = start.M_node+1;node<finish.M_node;++node)
		{
			destroy(*node,*node+buffer_size());
			dataAllocator::deallocate(*node,buffer_size());
		}
		if (start.M_node!=finish.M_node)
		{
			destroy(start.M_cur,start.M_last);
			destroy(finish.M_first,finish.M_cur);
			dataAllocator::deallocate(finish.M_first,buffer_size());
		}
		else
		{
			destroy(start.M_cur,finish.M_cur);
		}

		finish = start;
	}

	template<class T>
	typename deque<T>::iterator deque<T>::erase(iterator position)
	{
		iterator next = position;
		++next;
		difference_type index = position-start;
		if (index<(size()>>1))
		{
			copy_backward(start,position,next);
		}
		else 
		{
			copy(next,finish,position);
			pop_back();
		}
		return start+index;
	}

	template<class T>
	typename deque<T>::iterator deque<T>::erase(iterator first,iterator last)
	{
		if (first==start&&last==finish)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before<difference_type(((size()-n)>>1)) )
			{
				copy_backward(start,first,last);
				iterator new_start = start + n;
				destroy(start,new_start);
				for(map_pointer cur = start.M_node;cur<new_start.M_node;++cur)
				{
					dataAllocator::deallocate(*cur,buffer_size());
				}
				start = new_start;
			}
			else
			{
				copy(last,finish,first);
				iterator new_finish = finish-n;
				destroy(new_finish,finish);
				for(map_pointer cur = new_finish.M_node+1;cur<=finish.M_node;++cur)
				{
					dataAllocator::deallocate(*cur,buffer_size());
				}
				finish = new_finish;
			}
			return start+elems_before;
		}
	}

	template<class T>
	typename deque<T>::iterator deque<T>::insert_aux(iterator position,const value_type& x)
	{
		difference_type index = position-start;
		if (index<(size()>>1))
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			position = start + index;
			iterator pos1 = position;
			++pos1;
			copy(front2,pos1,front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + index;
			copy_backward(position,back2,back1);
		}
		*position = x;
		return pos;
	}

	template<class T>
	typename deque<T>::iterator deque<T>::insert(iterator position,const value_type& x)
	{
		if (position.M_cur == start.M_cur)
		{
			push_front(x);
			return start;
		}
		else if (position.M_cur == finish.M_cur)
		{
			push_back(x);
			iterator temp = finish;
			return --temp;
		}
		else
		{
			return insert_aux(position,x);
		}
	}

	template<class T>
	void deque<T>::swap(deque& x)
	{
		using MINI_STL::swap;
		swap(start,x.start);
		swap(finish,x.finish);
		swap(map,x.map);
		swap(map_size,x.map_size);
	}

	template<class T>
	deque<T>& deque<T>::operator=(const deque& x)
	{
		const size_type len = size();
		if (this != &x)
		{
			if (len > x.size())
			{
				erase(copy(x.begin(),x.end(),start),finish);
			}
			else
			{
				copy(x.begin(),x.begin()+difference_type(len),start);
				const_iterator first = x.begin()+difference_type(len);
				const_iterator last = x.end();
				while (first!=last)
				{
					push_back(*(first++));
				}
			}
		}
		return *this;
	}


	template<class T>
	inline bool operator==(const deque<T>& x,const deque<T>& y)
	{
		if (y.size()!=x.size())
		{
			return false;
		}
		auto first1 = x.begin();
		auto last1 = x.end();
		auto first2 = y.begin();
		auto last2 = y.end();
		while (first1 != last1 && first2 != last2 && *first1 == *first2)
		{
		++first1;
		++first2;
		}
		return first1==last1&&first2==last2;
	}

	template<class T>
	inline bool operator!=(const deque<T>&x,const deque<T>& y)
	{
		return !(x==y);
	}

	template<class T>
	inline bool operator<(const deque<T>& x,const deque<T>& y)
	{
		return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
	}

	template<class T>
	inline bool operator>(const deque<T>& x,const deque<T>& y)
	{
		return y<x;
	}

	template<class T>
	inline bool operator<=(const deque<T>& x,const deque<T>& y)
	{
		return !(x>y);
	}

	template<class T>
	inline bool operator>=(const deque<T>& x,const deque<T>& y)
	{
		return !(x<y);
	}


}
#endif