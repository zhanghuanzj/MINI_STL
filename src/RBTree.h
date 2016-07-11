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

	//Z指向需要删除的节点，Y指向实质结构上被删除的
	inline RBTree_node_base* rebalance_for_erase(RBTree_node_base* z,RBTree_node_base*& root,
		RBTree_node_base*& leftmost,RBTree_node_base*& rightmost)
	{
	  	RBTree_node_base* y = z;
	  	RBTree_node_base* x = 0;
	  	RBTree_node_base* x_parent = 0;
	  	//y指向最终要删除的节点，x为y的子节点
	  	if (y->left == 0)     	// z 至多有一个非空子节点
	   		x = y->right;     
	  	else
	    	if (y->right == 0)  // z 只有一个左子节点
	      		x = y->left;    
		   	else 
		   	{              		// z 有两个非空子节点，y为z的后继
		      	y = y->right;  
		      	while (y->left != 0)
		        	y = y->left;
		      	x = y->right;
		    }
	  	if (y != z) 
	  	{   
	  		//swap(y,z)      
	    	z->left->parent = y; 		//1
	   	 	y->left = z->left;			//2
	    	if (y != z->right) 
	    	{
	    		//调节x_parent及x
	      		x_parent = y->parent;
	      		if (x) x->parent = y->parent;  
	      		y->parent->left = x; 

	      		y->right = z->right;	//3
	      		z->right->parent = y;	//4
	   	 	}
	   		else  //后继为右子节点
	      		x_parent = y;  
	    	if (root == z)              //5
	      		root = y;
	    	else if (z->parent->left == z)
	      		z->parent->left = y;
	    	else 
	      		z->parent->right = y;
	    	y->parent = z->parent;      //6
	    	MINI_STL::swap(y->color, z->color);
	    	y = z;
	    	// y现在指向最终要删除的节点
	  	}
	  	else 	// y == z
	  	{     
	  		//调整x及x_parent                   
	    	x_parent = y->parent;
	    	if (x) x->parent = y->parent;   
	    	if (root == z)
	      		root = x;
	    	else 
	      		if (z->parent->left == z)
	        		z->parent->left = x;
	      		else
	        		z->parent->right = x;
	        //更新最左节点	
	    	if (leftmost == z) 
	      		if (z->right == nullptr)       	 
	        		leftmost = z->parent;
	      		else
	        		leftmost = RBTree_node_base::minimum(x);
	        //更新最右节点
	    	if (rightmost == z)  
	      		if (z->left == 0)        		 
	        		rightmost = z->parent;  
	      		else                    
	        		rightmost = RBTree_node_base::maximum(x);
	  	}
	  	if (y->color != red) 	//删除黑色节点需要调整
	  	{ 
	    	while (x != root && (x == 0 || x->color == black))
	      		if (x == x_parent->left) 
	      		{
	        		RBTree_node_base* w = x_parent->right;//x的兄弟节点
	        		//case1:w为红色，则w的儿子必然全黑，w父亲x_parent也为黑
	        		//改变x_parent与w的颜色，同时对x_parent做一次左旋，这样就将情况1转变为情况2,3,4的一种
	        		if (w->color == red) 
	        		{
	          			w->color = black;
	          			x_parent->color = red;
	          			rotate_left(x_parent, root);
	          			w = x_parent->right;
	        		}
	        		//case2:w的双子均为黑，将w置黑
			        if ((w->left == 0 || w->left->color == black) && (w->right == 0 ||w->right->color == black)) 
			        {
			          	w->color = red;
			          	x = x_parent;
			          	x_parent = x_parent->parent;
			        } 
			        else 
			        {
			        	//case3:w左红右黑，交换w与左孩子的颜色，对w进行右旋，进行4
	          			if (w->right == 0 ||w->right->color == black) 
	          			{
	            			if (w->left) w->left->color = black;
					            w->color = red;
					        rotate_right(w, root);
					        w = x_parent->right;
	          			}
	          			//case4:右为红
	          			//交换w与父亲p颜色，同时对p做左旋。这样左边缺失的黑色就补回来了，同时，将w的右儿子置黑，这样左右都达到平衡
	          			w->color = x_parent->color;
	          			x_parent->color = black;
	          			if (w->right) w->right->color = black;
	          			rotate_left(x_parent, root);
	          			break;
	        		}
	      		} 
	      		else 
	      		{                  // same as above, with right <-> left.
		        	RBTree_node_base* w = x_parent->left;
		        	if (w->color == red) 
		        	{
		          		w->color = black;
		          		x_parent->color = red;
		          		rotate_right(x_parent, root);
		          		w = x_parent->left;
		        	}
		        	if ((w->right == 0 || w->right->color == black) &&(w->left == 0 || w->left->color == black)) 
		        	{
				          w->color = red;
				          x = x_parent;
				          x_parent = x_parent->parent;
		        	} 
		        	else 
		        	{
		          		if (w->left == 0 || w->left->color == black) 
		          		{
		            		if (w->right) w->right->color = black;
		            		w->color = red;
		            		rotate_left(w, root);
		            		w = x_parent->left;
		          		}
		          		w->color = x_parent->color;
						x_parent->color = black;
						if (w->left) w->left->color = black;
						rotate_right(x_parent, root);
						break;
		        	}
	    		}
		    //将x置黑
		    if (x) x->color = black;
	  	}
	  	return y;
	}


	template <class Key,class Value,class KeyOfValue,class Compare>
	class RBTree
	{
	public:
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
	  	static const Key& getKey(Link_type x){ return KeyOfValue()(value(x)); }
	  	static RBTree_color& color(Link_type x){ return (RBTree_color&)(x->color); }

	  	static Link_type& left(base_ptr x){ return (Link_type&)(x->left); }
	  	static Link_type& right(base_ptr x){ return (Link_type&)(x->right); }
	  	static Link_type& parent(base_ptr x){ return (Link_type&)(x->parent); }
	  	static reference value(base_ptr x){ return ((Link_type)x)->data; }
	  	static const Key& getKey(base_ptr x){ return KeyOfValue()(value(Link_type(x)));} 
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
		void erase_recursive(Node* x);
		Node* copy_tree(Node* x,Node* p);
		void clear(); 
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
		RBTree& operator=(const RBTree& x);
		~RBTree(){clear();destroy_node(header);}

		iterator begin() {return leftmost();}
		iterator end() {return header;}
		const_iterator begin()const {return leftmost();}
		const_iterator end()const {return header;}
		void swap(RBTree& t)
		{ 
			MINI_STL::swap(header,t.header); 
			MINI_STL::swap(node_count,t.node_count);
			MINI_STL::swap(key_compare,t.key_compare);
		}

		size_type size()const {return node_count;}
		bool empty()const {return node_count==0;}
		iterator find(const Key& k);
		const_iterator find(const Key& k)const;
		size_type count(const Key& k)const;
		//插入
		iterator insert_equal(const value_type& v);
		void insert_equal(const_iterator first,const_iterator last);
		void insert_equal(const value_type* first,const value_type* last);
		template <class InputIterator>
		void insert_equal(InputIterator first,InputIterator last);
		pair<iterator,bool> insert_unique(const value_type& v);
		void insert_unique(const_iterator first,const_iterator last);
		void insert_unique(const value_type* first,const value_type* last);
		template <class InputIterator>
		void insert_unique(InputIterator first,InputIterator last);

		//删除
		void erase(iterator position);
		size_type erase(const key_type& x);
		void erase(iterator first, iterator last);
		void erase(const Key* first,const Key* last);
		
		
		//边界
		iterator lower_bound(const Key& k);
		const_iterator lower_bound(const Key& k)const;
		iterator upper_bound(const Key& k);
		const_iterator upper_bound(const Key& k)const;
		pair<iterator,iterator> equal_range(const Key& key);
		pair<const_iterator,const_iterator> equal_range(const Key& key)const;
		
	};

	template <class Key,class Value,class KeyOfValue,class Compare>
	RBTree< Key, Value, KeyOfValue, Compare>& RBTree< Key, Value, KeyOfValue, Compare>::operator=
	(const RBTree< Key, Value, KeyOfValue, Compare>&x)
	{
		if (this!=&x)
		{
			clear();
			node_count = 0;
			key_compare = x.key_compare;
			if (x.root() == nullptr)
			{
				root() = nullptr;
				leftmost() = header;
				rightmost() = header;
			}
			else
			{
				root() = copy_tree(x.root(),header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
				node_count = x.node_count;
			}
		}
		return *this;
	}
	
	//pos为新值插入位置，parent为插入点父节点
	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::insert(base_ptr pos,base_ptr par,const value_type& v)        
	{
		Node* x =(Node*)pos;
		Node* y = (Node*)par;
		Node* z;
		if (y==header || x!=nullptr || key_compare(KeyOfValue()(v),getKey(y)))
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
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::insert_equal(const value_type& v)   
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			y = x;
			x = key_compare(KeyOfValue()(v),getKey(x))?left(x):right(x);
		}
		return insert(x,y,v);
	}     

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::insert_equal(const value_type* first,const value_type* last)  
	{
		while(first!=last)
		{
			insert_equal(*first);
			++first;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::insert_equal(const_iterator first,const_iterator last)  
	{
		while(first!=last)
		{
			insert_equal(*first);
			++first;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	template <class InputIterator>
	void RBTree< Key, Value, KeyOfValue, Compare>::insert_equal(InputIterator first,InputIterator last)  
	{
		while(first!=last)
		{
			insert_equal(*first);
			++first;
		}
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
			isLeft = key_compare(KeyOfValue()(v),getKey(x));
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
		if (key_compare(getKey(j.node),KeyOfValue()(v)))
		{
			return pair<iterator,bool>(insert(x,y,v),true);
		}
		return pair<iterator,bool>(j,false);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::insert_unique(const value_type* first,const value_type* last)  
	{
		while(first!=last)
		{
			insert_unique(*first);
			++first;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::insert_unique(const_iterator first,const_iterator last)  
	{
		while(first!=last)
		{
			insert_unique(*first);
			++first;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	template <class InputIterator>
	void RBTree< Key, Value, KeyOfValue, Compare>::insert_unique(InputIterator first,InputIterator last)  
	{
		while(first!=last)
		{
			insert_unique(*first);
			++first;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::find(const Key& k)
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (!key_compare(getKey(x),k))
			{	
				y = x,x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		iterator j = iterator(y);
		return (j==end() || key_compare(k,getKey(j.node)))?end():j;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::const_iterator
	RBTree< Key, Value, KeyOfValue, Compare>::find(const Key& k)const
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (!key_compare(getKey(x),k))
			{	
				y = x,x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		iterator j = iterator(y);
		return (j==end() || key_compare(k,getKey(j.node)))?end():j;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::size_type
	RBTree< Key, Value, KeyOfValue, Compare>::count(const Key& k)const
	{
		pair<const_iterator,const_iterator> p = equal_range(k);
		return distance(p.first,p.second);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::erase(iterator position)
	{
		Node* y = (Node*)rebalance_for_erase(position.node,header->parent,header->left,header->right);
		destroy_node(y);
		--node_count;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::size_type
	RBTree< Key, Value, KeyOfValue, Compare>::erase(const Key& x)
	{
		pair<iterator,iterator> p = equal_range(x);
		size_type n = distance(p.first,p.second);
		erase(p.first,p.second);
		return n;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::erase_recursive(Node* x)
	{
		while(x!=nullptr)
		{
			erase_recursive(right(x));
			Node* y = left(x);
			destroy_node(x);
			x = y;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::erase(iterator first,iterator last)
	{
		if (first==begin()&&last==end())
		{
			clear();
		}
		else
		{
			while(first!=last)
				erase(first++);
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::erase(const Key* first,const Key* last)
	{
		while(first!=last)
			erase(*first++);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::Node* 
	RBTree< Key, Value, KeyOfValue, Compare>::copy_tree(Node* x,Node* p)
	{
		Node* root = clone_node(x);
		root->parent = p;

		if (x->right)
		{
			root->right = copy_tree(right(x),root);
		}
		p = root;
		x = left(x);

		while(x!=nullptr)
		{
			Node* y = clone_node(x);
			p->left = y;
			y->parent = p;
			if (x->right)
			{
				y->right = copy_tree(right(x),y);
			}
			p = y;
			x = left(x);
		}
		return root;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	void RBTree< Key, Value, KeyOfValue, Compare>::clear()
	{
		if (node_count!=0)
		{
			erase_recursive(root());
			leftmost() = header;
			root() = nullptr;
			rightmost() = header;
			node_count = 0;
		}
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::lower_bound(const Key& k)
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (!key_compare(getKey(x),k)) //x>=k
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		return iterator(y);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::const_iterator
	RBTree< Key, Value, KeyOfValue, Compare>::lower_bound(const Key& k)const
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (!key_compare(getKey(x),k)) //x>=k
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		return const_iterator(y);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::const_iterator
	RBTree< Key, Value, KeyOfValue, Compare>::upper_bound(const Key& k)const
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (key_compare(k,getKey(x))) //x>k,相同的是放在左边的
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		return const_iterator(y);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	typename RBTree< Key, Value, KeyOfValue, Compare>::iterator
	RBTree< Key, Value, KeyOfValue, Compare>::upper_bound(const Key& k)
	{
		Node* y = header;
		Node* x = root();
		while(x!=nullptr)
		{
			if (key_compare(k,getKey(x))) //k>x
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		return iterator(y);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline pair<typename RBTree< Key, Value, KeyOfValue, Compare>::iterator,
				typename RBTree< Key, Value, KeyOfValue, Compare>::iterator>
	RBTree< Key, Value, KeyOfValue, Compare>::equal_range(const Key& key)
	{
		return pair<iterator,iterator>(lower_bound(key),upper_bound(key));
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline pair<typename RBTree< Key, Value, KeyOfValue, Compare>::const_iterator,
				typename RBTree< Key, Value, KeyOfValue, Compare>::const_iterator>
	RBTree< Key, Value, KeyOfValue, Compare>::equal_range(const Key& key)const
	{
		return pair<const_iterator,const_iterator>(lower_bound(key),upper_bound(key));
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline bool 
	operator==(const RBTree< Key, Value, KeyOfValue, Compare>& x, 
	           const RBTree< Key, Value, KeyOfValue, Compare>& y)
	{
	  return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline bool 
	operator<(const RBTree< Key, Value, KeyOfValue, Compare>& x, 
	          const RBTree< Key, Value, KeyOfValue, Compare>& y)
	{
	  return lexicographical_compare(x.begin(), x.end(), 
	                                 y.begin(), y.end());
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline bool 
	operator!=(const RBTree< Key, Value, KeyOfValue, Compare>& x, 
	           const RBTree< Key, Value, KeyOfValue, Compare>& y) {
	  return !(x == y);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline bool 
	operator>(const RBTree< Key, Value, KeyOfValue, Compare>& x, 
	          const RBTree< Key, Value, KeyOfValue, Compare>& y) {
	  return y < x;
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline bool 
	operator<=(const RBTree< Key, Value, KeyOfValue, Compare>& x, 
	           const RBTree< Key, Value, KeyOfValue, Compare>& y) {
	  return !(y < x);
	}

	template <class Key,class Value,class KeyOfValue,class Compare>
	inline bool 
	operator>=(const RBTree< Key, Value, KeyOfValue, Compare>& x, 
	           const RBTree< Key, Value, KeyOfValue, Compare>& y) {
	  return !(x < y);
	}

}
#endif