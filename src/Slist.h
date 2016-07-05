#ifndef _MINI_STL_SLIST_H_
#define _MINI_STL_SLIST_H_

#include "Iterator.h"
#include "Allocator.h"

namespace MINI_STL
{
	struct slist_node_base
	{
		slist_node_base *next;
	};

	template<class T>
	struct slist_node : public slist_node_base
	{
		T data;
	};

	inline slist_node_base* slist_make_link(slist_node_base* prev_node,slist_node_base* new_node)
	{
		new_node->next = prev_node->next;
		prev_node->next = new_node;
		return new_node;
	}

	inline slist_node_base* slist_previous(slist_node_base* head,const slist_node_base* pos)
	{
		while(head && head->next!=pos)
			head = head->next;
		return head;
	}

	inline const slist_node_base* slist_previous(const slist_node_base* head,const slist_node_base* pos)
	{
		while(head && head->next!=pos)
			head = head->next;
		return head;
	}

	inline size_t slist_size(slist_node_base* node)
	{
		size_t result = 0;
		while(node!=nullptr)
		{
			++result;
			node = node->next;
		}
		return result;
	}

	struct slist_iterator_base
	{
		typedef size_t               size_type;
		typedef ptrdiff_t            difference_type;
		typedef forward_iterator_tag iterator_category;

		slist_node_base* node;

		slist_iterator_base(slist_node_base* x):node(x){}
		void incr(){node = node->next;}

		bool operator==(const slist_iterator_base& x)const
		{
			return node == x.node;
		}
		bool operator!=(const slist_iterator_base& x)const
		{
			return node != x.node;
		}
	};

	template <class T,class Ref,class Ptr>
	struct slist_iterator : public slist_iterator_base
	{
		typedef slist_iterator<T,T&,T*>					iterator;
		typedef slist_iterator<T,const T&,const T*> 	const_iterator;

		typedef T   			value_type;
		typedef Ptr     		pointer;
		typedef Ref				reference;
		typedef slist_node<T>	Node;

		slist_iterator(Node* x):slist_iterator_base(x){}
		slist_iterator():slist_iterator_base(nullptr){}
		slist_iterator(const slist_iterator& x):slist_iterator_base(x.node){}

		reference operator*() {return ((Node*)node)->data;}
		pointer operator->(){return &(operator*());}

		slist_iterator& operator++()
		{
			incr();
			return *this;
		}

		slist_iterator operator++(int)
		{
			slist_iterator temp = *this;
			incr();
			return temp;
		}
	};

	template <class T>
	class slist
	{
	public:
		typedef T									value_type;
		typedef T*									pointer;
		typedef slist_iterator<T,T&,T*>				iterator;
		typedef slist_iterator<T,const T&,const T*>	const_iterator;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;

	private:
		typedef slist_node<T>						list_node;
		typedef slist_node_base						list_node_base;
		typedef slist_iterator_base					iterator_base;
		typedef Allocator<slist_node<T>>			nodeAllocator;

		static list_node* create_node(const value_type& x)
		{
			list_node* node = nodeAllocator::allocate();
			construct(&node->data,x);
			node->next = nullptr;
			return node;
		}

		static void destroy_node(list_node* node)
		{
			destroy(&node->data);
			nodeAllocator::deallocate(node);
		}

	private:
		list_node_base head;

	public:
		slist() {head.next = nullptr;}
		~slist() {clear();}

		iterator begin() {return iterator((list_node*)head.next);}
		iterator end() {return iterator(nullptr);}
		size_type size()const {return slist_size(head.next);}
		bool empty()const {return head.next==nullptr;}

		reference front() { return ((list_node*)head.next)->data;}
		void push_front(const value_type& x) {slist_make_link(&head,create_node(x));}
		void pop_front() 
		{
			list_node* temp = (list_node*)head.next;
			head.next = head.next->next;
			destroy_node(temp);
		}

		iterator insert(iterator position,const value_type& x)
		{
			list_node_base* pre = slist_previous(&head,position.node);
			list_node_base* node = slist_make_link(pre,create_node(x));
			return iterator((list_node*)node);
		}

		iterator erase(iterator position)
		{
			list_node_base* pre = slist_previous(&head,position.node);
			pre->next = pre->next->next;
			destroy_node((list_node*)position.node);
			return iterator((list_node*)pre->next);
		}

		void clear()
		{
			while (head.next!=nullptr)
			{
				list_node* temp = (list_node*)head.next;
				head.next = head.next->next;
				destroy_node(temp);
			}
		}
		void swap(slist& x)
		{
			MINI_STL::swap(head.next,x.head.next);
		}
	};
}

#endif