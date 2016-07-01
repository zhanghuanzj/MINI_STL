#ifndef _MINI_STL_DEQUE_H_
#define _MINI_STL_DEQUE_H_


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

	difference_type operator(const deque_iterator& x)const
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
	template<class InputIterator>
	void range_initialize(InputIterator first,InputIterator last,input_iterator_tag);
	template<class ForwardIterator>
	void range_initialize(ForwardIterator first,ForwardIterator last,forward_iterator_tag);
	template<class Integer>
	void deque_aux(Integer n,Integer val,_true_type);
	template<class InputIterator>
	void deque_aux(InputIterator first,InputIterator last,_false_type);
public:
	deque():start(),finish(),map_size(0),map(0){}
	deque(int n,const T& val=T()):start(),finish(),map(0),map_size(0){create_map_and_nodes(n); fill_initialize(value);}
	deque(const deque& x){	create_map_and_nodes(x.size());  uninitialized_copy(x.begin(),x.end(),start);}
	template<class InputIterator>
	deque(InputIterator first,InputIterator last);
public:
	iterator start;
	iterator finish;

	map_pointer map;
	size_type map_size;


};

template<class InputIterator>
void range_initialize(InputIterator first,InputIterator last,input_iterator_tag)
{
	create_map_and_nodes(0);
	for(;first!=last;++first)
		push_back(*first);
}

template<class ForwardIterator>
void range_initialize(ForwardIterator first,ForwardIterator last,forward_iterator_tag)
{
	size_type n = distance(first,last);
	create_map_and_nodes(n);
	map_pointer cur;
	for(cur=start.M_node;cur<finish.M_node;++cur)
	{
		ForwardIterator mid = first;
		advance(mid,buffer_size());
		uninitialized_copy(first,mid,*cur);
		first = mid;
	}
	uninitialized_copy(first,last,finish.M_first);
}

template<class Integer>
void deque<T>::deque_aux(Integer n,Integer val,_true_type)
{
	create_map_and_nodes(n);
	fill_initialize(x);
}

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
		uninitialized_fill(*cur,*cur+buffer_size(),value);
	}
	uninitialized_fill(finish.M_first,finish.M_cur,value);
}

template<class T>
void deque<T>::create_map_and_nodes(size_type num_elements)
{
	size_type num_nodes = num_elements/buffer_size()+1;
	map_size = max(initial_map_size,num_nodes+2);
	map = mapAllocator::allocate(map_size);

	map_pointer nstart = map + (map_size - num_nodes)/2;
	map_pointer nfinish = map + num_nodes - 1;
	map_pointer cur;

	for(cur=nstart;cur<=nfinish;++cur)
	{
		*cur = allocate_nodes();
	}

	start.set_node(nstart);
	start.cur = start.M_first;
	finish.set_node(nfinish);
	finish.cur = finish.M_first + num_elements%buffer_size();
}

template<class T>
template<class InputIterator>
deque<T>::deque(InputIterator first,InputIterator last)
{
	typedef typename _is_Integer<InputIterator>::_Integral _Integral;
	deque_aux(first,last,_Integral());
}
#endif