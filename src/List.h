#ifndef _MINI_STL_LIST_H_
#define _MINI_STL_LIST_H_

#include "Allocator.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "Uninitialized.h"
#include "TypeTraits.h"
#include "Algorithm.h"

namespace MINI_STL
{
	template <class T>
	struct list_node
	{
		list_node *prev;
		list_node *next;
		T data;
		list_node():prev(NULL),next(NULL),data(){}
	};

	template<class T>
	struct list_iterator :public iterator<bidirectional_iterator_tag, T>{
		template<class T>
		friend class list;
	public:
		typedef list_node<T>* list_node_pointer;
		list_node_pointer node;
	public:
		list_iterator(list_node_pointer ptr = nullptr) :node(ptr){}

		list_iterator& operator++();
		list_iterator operator++(int);
		list_iterator& operator --();
		list_iterator operator --(int);
		T& operator *()const { return node->data; }
		T* operator ->()const { return &(operator*()); }

		bool operator ==(const list_iterator<T>& it)const{return node==it.node;}
		bool operator !=(const list_iterator<T>& it)const{return node!=it.node;}
	};

	template<class T>
	class list
	{
	protected:
		typedef list_node<T> listNode;
		typedef Allocator<list_node<T>> nodeAllocator;
	public:
		typedef T value_type;
		typedef list_iterator<T> iterator;
		typedef list_iterator<const T> const_iterator;
		typedef reverse_iterator_t<iterator> reverse_iterator;
		typedef reverse_iterator_t<const iterator> const_reverse_iterator;
		typedef T& reference;
		typedef size_t size_type;


	protected:
		//member
		listNode *node;
		void empty_initialize();
	public:

		//constructor
		list();
		list(int n, const T& val);

		template <class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& l);
		list& operator = (const list& l);
		~list();

		iterator begin() {return node->next;}
		iterator begin()const {return node->next;}
		reverse_iterator rbegin() {return reverse_iterator(node);}
		const_reverse_iterator crbegin()const {return const_reverse_iterator(node);}
		reverse_iterator rend() {return reverse_iterator(node->next);}
		const_reverse_iterator crend()const {return const_reverse_iterator(node->next);}
		iterator end() {return node;}
		iterator end()const {return node;}
		bool empty() const { return node->next==node;}
		size_type size()const{ return MINI_STL::distance(begin(),end());}
		reference front(){return *begin();}
		reference back(){return *(--end());}
		void swap(list &x){MINI_STL::swap(node,x.node);}

		//配置、释放、构造、销毁一个节点
		listNode* acquire_node_space(){return nodeAllocator::allocate();}
		void release_node_space(listNode *node){nodeAllocator::deallocate(node);}
		listNode* create_node(const T& x);
		void destroy_node(listNode *node);

		//插入、删除
		iterator insert(iterator position,const T &x);
		void push_front(const T& x){insert(begin(),x);}
		void push_back(const T& x){insert(end(),x);}
		iterator erase(iterator position);
		void pop_front(){erase(begin());}
		void pop_back(){erase(--end());}
		void clear();
		void remove(const T& value);
		void unique();

		//转移、拼接、排序、合并、反转
	protected:
		void transfer(iterator position,iterator first,iterator last);
	public:
		void splice(iterator position,list& x){if(!x.empty()) transfer(position,x.begin(),x.end());}
		void splice(iterator position,list&,iterator i);
		void splice(iterator position,list&,iterator first,iterator last){if (first!=last) transfer(position,first,last);}
		void merge(list &x);
		void reverse();
		void sort();
	};

	template<class T>
	void list<T>::empty_initialize()
	{
		node = acquire_node_space();
		node->next = node;
		node->prev = node;
	}

	template<class T>
	list<T>::list()
	{
		empty_initialize();
	}

	template<class T>
	list<T>::list(int n,const T& val)
	{
		empty_initialize();
		while (n--)
		{
			push_back(val);
		}
	}

	template<class T>
	template <class InputIterator>
	list<T>::list(InputIterator first, InputIterator last)
	{
		empty_initialize();
		while (first!=last)
		{
			push_back(*first);
			++first;
		}
	}

	template<class T>
	list<T>::list(const list&x)
	{
		empty_initialize();
		iterator first = x.begin();
		iterator last = x.end();
		while (first!=last)
		{
			push_back(*first);
			++first;
		}
	}

	template<class T>
	list<T>& list<T>::operator=(const list&x)
	{
		if (this!=&x)
		{
			clear();
			iterator first = x.begin();
			iterator last = x.end();
			while (first!=last)
			{
				push_back(*first);
				++first;
			}
		}
		return *this;
	}

	template<class T>
	list<T>::~list()
	{
		clear();
		release_node_space(node);
	}

	template<class T>
	typename list<T>::listNode* list<T>::create_node(const T& x)
	{
		listNode* node = acquire_node_space();
		construct(&node->data,x);
		return node;
	}

	template<class T>
	void list<T>::destroy_node(listNode *node)
	{
		destroy(&node->data);
		release_node_space(node);
	}

	template<class T>
	typename list<T>::iterator list<T>::insert(iterator position,const T &x)
	{
		listNode *temp = create_node(x);
		temp->next = position.node;
		temp->prev = position.node->prev;
		position.node->prev->next = temp;
		position.node->prev = temp;
		return temp;
	}

	template<class T>
	typename list<T>::iterator list<T>::erase(iterator position)
	{
		listNode *next_node = position.node->next;
		listNode *prev_node = position.node->prev;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(position.node);
		return next_node;
	}

	template<class T>
	void list<T>::clear()
	{
		listNode *cur = node->next;
		while(cur!=node)
		{
			listNode *temp = cur;
			cur = cur->next;
			destroy_node(temp);
		}
		node->next = node;
		node->prev = node;
	}

	template<class T>
	void list<T>::remove(const T& value)
	{
		iterator first = begin();
		iterator last = end();
		while(first!=last)
		{
			if (*first==value)
			{
				first = erase(first);
			}
			else
			{
				++first;
			}
		}
	}

	template<class T>
	void list<T>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first==last)
		{
			return;
		}
		iterator next = first;
		while(++next!=last)
		{
			if (*first==*next)
			{
				erase(next);
			}
			else
			{
				first = next;
			}
			next = first;
		}
	}

	template<class T>
	void list<T>::transfer(iterator position,iterator first,iterator last)
	{
		listNode* pos = position.node;
		listNode* fir = first.node;
		listNode* las = last.node;
		las->prev->next = pos;
		fir->prev->next = las;
		pos->prev->next = fir;
		listNode *temp = pos->prev;
		pos->prev = las->prev;
		las->prev = fir->prev;
		fir->prev = temp;
	}

	template<class T>
	void list<T>::splice(iterator position,list&,iterator i)
	{
		iterator j = i;
		++j;
		if (position==i||position==j)
		{
			return;
		}
		transfer(position,i,j);
	}

	template<class T>
	void list<T>::merge(list &x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while(first1!=last1&&first2!=last2)
		{
			if (*first2<*first1)
			{
				iterator next = first2;
				transfer(first1,first2,++next);
				first2 = next;
			}
			else
			{
				++first1;
			}
		}
		if (first2!=last2)
		{
			transfer(last1,first2,last2);
		}
	}

	template<class T>
	void list<T>::reverse()
	{
		if (node->next==node||node->next->next==node)
		{
			return;
		}
		iterator first = begin();
		++first;
		while(first!=end())
		{
			iterator old = first;
			++first;
			transfer(begin(),old,first);
		}
	}

	template<class T>
	void list<T>::sort()
	{
		if (node->next==node||node->next->next==node)
		{
			return;
		}
		list carry;
		list counter[64];
		int fill = 0;
		while(!empty())
		{
			carry.splice(carry.begin(),*this,begin());
			int i=0;
			while(i<fill&&!counter[i].empty())
			{
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i==fill)
			{
				++fill;
			}
		}
		for (int i = 1; i < fill; ++i)
		{
			counter[i].merge(counter[i-1]);
		}
		swap(counter[fill-1]);
	}


	//list_iterator
	template<class T>
	list_iterator<T>& list_iterator<T>::operator++()
	{
		node = node->next;
		return *this;
	}

	template<class T>
	list_iterator<T> list_iterator<T>::operator++(int)
	{
		auto temp = *this;
		++*this;
		return temp;
	}

	template<class T>
	list_iterator<T>& list_iterator<T>::operator--()
	{
		node = node->prev;
		return *this;
	}

	template<class T>
	list_iterator<T> list_iterator<T>::operator--(int)
	{
		auto temp = *this;
		--*this;
		return temp;
	}
}

#endif