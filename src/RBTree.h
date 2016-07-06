#ifndef _MINI_STL_RBTREE_H_
#define _MINI_STL_RBTREE_H_

#include "Pair.h"
#include "Construct.h"
#include "Iterator.h"
#include "Allocator.h"

namespace MINI_STL
{
	typedef bool RBTree_color;
	const RBTree_color red = false;
	const RBTree_color black = true;

	struct RBTree_node_base
	{
		RBTree_color color;
		RBTree_node_base* parent;
		RBTree_node_base* left;
		RBTree_node_base* right;

		static RBTree_node_base* minimum(RBTree_node_base* x)
		{
			while(x->left!=nullptr)
				x = x->left;
			return x;
		}

		static RBTree_node_base* maximum(RBTree_node_base* x)
		{
			while(x->right!=nullptr)
				x = x->right;
			return x;
		}
	};

	template <class T>
	struct RBTree_node : public RBTree_node_base
	{
		typedef RBTree_node<T> Node;
		T data;
	};

	struct RBTree_iterator_base
	{
		typedef RBTree_node_base*	base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t	difference_type;

		base_ptr node;

		void increment()
		{
			if (node->right != nullptr)
			{
				node = node->right;
				while(node->left != nullptr)
					node = node->left;
			}
			else
			{
				base_ptr y = node->parent;
				while(node == y->right)
				{
					node = y;
					y = y->parent;
				}
				if (node->right != y)
				{
					node = y;
				}
			}
		}

		void decrement()
		{
			if (node->color==red && node->parent->parent==node)
			{
				node = node->right;
			}
			else if (node->left!=nullptr)
			{
				base_ptr y = node->left;
				while(y->right != nullptr)
					y = y->right;
				node = y;
			}
			else
			{
				base_ptr y = node->parent;
				while(y->left == node)
				{
					node = y;
					y = y->parent;
				}	
				node = y;
			}
		}
	};

	template <class T,class Ref,class Ptr>
	struct RBTree_iterator :public RBTree_iterator_base
	{
		typedef T  		value_type;
		typedef Ref		reference;
		typedef Ptr 	pointer;
		typedef RBTree_iterator<T,T&,T*> 	iterator;
		typedef RBTree_iterator<T,const T&,const T*> const_iterator;
		typedef RBTree_node<T> Node;

		RBTree_iterator(){}
		RBTree_iterator(Node* x) {node = x;}         //子类指针赋给父类指针
		RBTree_iterator(const iterator& it) {node = it.node;}

		reference operator*() const {return ((Node*)node)->data;}
		pointer operator->() const {return &(operator*());}

		RBTree_iterator& operator++() {increment(); return *this;}
		RBTree_iterator operator++(int) 
		{
			RBTree_iterator temp = *this;
			increment();
			return temp;
		}
		RBTree_iterator& operator--() {decrement(); return *this;}
		RBTree_iterator operator--(int)
		{
			RBTree_iterator temp = *this;
			decrement();
			return temp;
		}

	};

	inline bool operator==(const RBTree_iterator_base& x,const RBTree_iterator_base& y)
	{
		return x.node == y.node;
	}

	inline bool operator!=(const RBTree_iterator_base& x,const RBTree_iterator_base& y)
	{
		return x.node != y.node;
	}

	inline void rotate_left(RBTree_node_base* x,RBTree_node_base*& root)
	{
		RBTree_node_base* y = x->right;
		x->right = y->left;
		if (y->left!=nullptr)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x==root)
		{
			root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	inline void rotate_right(RBTree_node_base* x,RBTree_node_base*& root)
	{
		RBTree_node_base* y = x->left;
		x->left = y->right;
		if (y->right!=nullptr)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x==root)
		{
			root = y;
		}
		else if (x==x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->right = x;
		x->parent = y;
	}

	//x为新插入点
	inline void rebalance(RBTree_node_base* x,RBTree_node_base*& root)
	{
		x->color = red;
		while(x!=root && x->parent->color==red)  //不为根节点&&父节点为红色（违反红黑树定义）
		{
			if(x->parent == x->parent->parent->left) //父为祖父的左子
			{
				RBTree_node_base* y = x->parent->parent->right;
				if (y && y->color==red)
				{
					x->parent->color = black;
					y->color = black;
					x->parent->parent->color = red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->right)  //内，需预先旋转成外
					{
						x = x->parent;
						rotate_left(x,root);
					}
					x->parent->color = black;
					x->parent->parent->color = red;
					rotate_right(x->parent->parent,root);
				}
			}
			else
			{
				RBTree_node_base* y = x->parent->parent->left;
				if (y && y->color==red)
				{
					x->parent->color = black;
					y->color = black;
					x->parent->parent->color = red;
					x = x->parent->parent;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						rotate_right(x,root);
					}
					x->parent->color = black;
					x->parent->parent->color = red;
					rotate_left(x->parent->parent,root);
				}
			}
		}
		root->color = black;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	class RBTree
	{
	protected:
		typedef RBTree_node_base* base_ptr;
		typedef RBTree_node<Value> Node;
		typedef Allocator<Node> nodeAllocator;

	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Node*	Link_type;

	protected:
		Node* create_node(const value_type& x)
		{
			Node* temp = nodeAllocator::allocate();
			construct(&temp->data,x);
			return temp;
		}
		void destroy_node(Node* p)
		{
			destroy(&p->data);
			nodeAllocator::deallocate(p);
		}
		Node* clone_node(Node* x)
		{
			Node *temp = nodeAllocator::allocate();
			temp->color = x->color;
			temp->left = x->left;
			temp->right = x->right;
			return temp;
		}

		Link_type& root() const { return (Link_type&) header->parent; }
	  	Link_type& leftmost() const { return (Link_type&) header->left; }
	  	Link_type& rightmost() const { return (Link_type&) header->right; }

	  	static Link_type& left(Link_type x){ return (Link_type&)(x->left); }
	  	static Link_type& right(Link_type x){ return (Link_type&)(x->right); }
	  	static Link_type& parent(Link_type x){ return (Link_type&)(x->parent); }
	  	static reference value(Link_type x){ return x->data; }
	  	static const Key& key(Link_type x){ return KeyOfValue()(value(x)); }
	  	static RBTree_color& color(Link_type x){ return (RBTree_color&)(x->color); }

	  	static Link_type& left(base_ptr x){ return (Link_type&)(x->left); }
	  	static Link_type& right(base_ptr x){ return (Link_type&)(x->right); }
	  	static Link_type& parent(base_ptr x){ return (Link_type&)(x->parent); }
	  	static reference value(base_ptr x){ return ((Link_type)x)->data; }
	  	static const Key& key(base_ptr x){ return KeyOfValue()(value(Link_type(x)));} 
	  	static RBTree_color& color(base_ptr x){ return (RBTree_color&)(Link_type(x)->color); }

	  	static Link_type minimum(Link_type x) { return (Link_type)  RBTree_node_base::minimum(x); }
	  	static Link_type maximum(Link_type x){ return (Link_type) RBTree_node_base::maximum(x); }

	public:
		typedef RBTree_iterator<value_type,reference,pointer> iterator;
		typedef RBTree_iterator<value_type,const_reference,const_pointer> const_iterator;
	protected:
		size_type node_count;
		Node* 	header;
		Compare key_compare;

	private:
		iterator insert(base_ptr pos,base_ptr par,const value_type& v);
		Node* copy(Node* x,Node* y);
		void erase(Node* x);
		void init()
		{
			header = nodeAllocator::allocate();
			color(header) = red;
			root() = nullptr;
			leftmost() = header;
			rightmost() = header;
		}
	public:
		RBTree():node_count(0),header(nullptr),key_compare(){init();}
		RBTree(const Compare& comp):node_count(0),header(nullptr),key_compare(comp){init();}
		~RBTree(){}

		iterator begin() {return leftmost();}
		iterator end() {return header;}
		const_pointer begin()const {return leftmost();}
		const_iterator end()const {return header;}

		size_type size()const {return node_count;}
		pair<iterator,bool> insert_equal(const value_type& v);
		pair<iterator,bool> insert_unique(const value_type& v);
		iterator find(const Key& k);

	};

	//pos为新值插入位置，parent为插入点父节点
	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::insert(base_ptr pos,base_ptr par,const value_type& v)        
	{
		Node* x =(Node*)pos;
		Node* y = (Node*)par;
		Node* z;
		if (y==header || x!=nullptr || key_compare(KeyOfValue()(v),key(y)))
		{
			z = create_node(v);
			left(y) = z;
			if (y==header)
			{
				root() = z;
				rightmost() = z;
			}
			else if (y==leftmost())
			{
				leftmost() = z;
			}
		}
		else
		{
			z = create_node(v);
			right(y) = z;
			if (y==rightmost())
			{
				rightmost() = z;
			}
		}
		parent(z) = y;
		left(z) = nullptr;
		right(z) = nullptr;
		rebalance(z,header->parent);
		++node_count;
		return iterator(z);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	pair<typename RBTree< Key, Value, KeyOfValue, Compare>::iterator,bool>
	RBTree< Key, Value, KeyOfValue, Compare>::insert_equal(const value_type& v)   
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			y = x;
			x = key_compare(KeyOfValue()(v),key(x))?left(x):right(x);
		}
		return insert(x,y,v);
	}     

	template <class Key,class Value,class KeyOfValue,class Compare>
	pair<typename RBTree<Key,Value, KeyOfValue, Compare>::iterator,bool>
	RBTree< Key, Value, KeyOfValue, Compare>::insert_unique(const value_type& v)  
	{
		Node* y = header;
		Node* x = root();
		bool isLeft = true;
		while(x!=0)
		{
			y = x;
			isLeft = key_compare(KeyOfValue()(v),key(x));
			x = isLeft?left(x):right(x);
		}

		iterator j = iterator(y);
		if (isLeft)
		{
			if (j==begin())
			{
				return pair<iterator,bool>(insert(x,y,v),true);
			}
			else
			{
				--j;
			}
		}
		if (key_compare(key(j.node),KeyOfValue()(v)))
		{
			return pair<iterator,bool>(insert(x,y,v),true);
		}
		return pair<iterator,bool>(j,false);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::find(const Key& k)
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (!key_compare(key(x),k))
			{	
				y = x,x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		iterator j = iterator(y);
		return (j==end() || key_compare(k,key(j.node)))?end():j;
	}
}
#endif