#ifndef _MINI_STL_HASHTABLE_H_
#define _MINI_STL_HASHTABLE_H_

#include "Algorithm.h"
#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include "Vector.h"
#include "Pair.h"

namespace MINI_STL
{
	template<class Value>
	struct hashtable_node
	{
		hashtable_node* next;
		Value val;
	};

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	class hashtable;

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	struct hashtable_const_iterator;

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	struct hashtable_iterator
	{
		typedef hashtable<Value,Key,HashFcn,ExtractKey,EqualKey> hashtable;
		typedef hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> iterator;
		typedef hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> const_iterator;
		typedef hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Value& reference;
		typedef Value* pointer;

		node *cur;
		hashtable *ht;

		hashtable_iterator(){}
		hashtable_iterator(node *n,hashtable *table):cur(n),ht(table){}

		reference operator*()const{return cur->val;}
		pointer operator->()const{return &(operator*());}

		iterator& operator++();
		iterator operator++(int);

		bool operator==(const iterator& it)const{return cur==it.cur;}
		bool operator!=(const iterator& it)const{return cur!=it.cur;}
	};

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	struct hashtable_const_iterator
	{
		typedef hashtable<Value,Key,HashFcn,ExtractKey,EqualKey> hashtable;
		typedef hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> iterator;
		typedef hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> const_iterator;
		typedef hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef const Value& reference;
		typedef const Value* pointer;

		const node *cur;
		const hashtable *ht;

		hashtable_const_iterator(){}
		hashtable_const_iterator(const node *n,const hashtable *table):cur(n),ht(table){}
		hashtable_const_iterator(const iterator& it):cur(it.cur),ht(it.ht){}

		reference operator*()const{return cur->val;}
		pointer operator->()const{return &(operator*());}

		const_iterator& operator++();
		const_iterator operator++(int);

		bool operator==(const const_iterator& it)const{return cur==it.cur;}
		bool operator!=(const const_iterator& it)const{return cur!=it.cur;}
	};

	enum { num_primes = 28 };

	static const unsigned long prime_list[num_primes] =
	{
	  53ul,         97ul,         193ul,       389ul,       769ul,
	  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
	  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
	  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
	  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
	  1610612741ul, 3221225473ul, 4294967291ul
	};

	inline unsigned long next_prime(unsigned long n)
	{
		const unsigned long* first = prime_list;
		const unsigned long* last = prime_list + (int)num_primes;
		const unsigned long* pos = lower_bound(first,last,n);
		return pos==last?*(last-1):*pos;
	}

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	bool operator==(const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>& ht1,
					const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>& ht2);

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	class hashtable
	{
	public:
		typedef HashFcn hasher;
		typedef EqualKey key_equal;
		typedef Value value_type;
		typedef Key key_type;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

	private:
		typedef hashtable_node<Value> node;
	public:
		typedef Allocator<node> node_allocator;
		typedef hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> 	iterator;
		typedef hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>	const_iterator;
		friend struct hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>;
		friend struct hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>;
		template <class VL, class KY, class HF, class EX, class EQ>
  		friend bool operator== (const hashtable<VL, KY, HF, EX, EQ>&,
                          		const hashtable<VL, KY, HF, EX, EQ>&);
	private:
		node* getNode(){return node_allocator::allocate();}
		void putNode(node* p){node_allocator::deallocate(p);}
		node* new_node(const value_type& v)
		{
			node* n = getNode();
			n->next = nullptr;
			construct(&n->val,v);
			return n;
		}
		void delete_node(node* n)
		{
			destroy(&n->val);
			putNode(n);
		}

	private:
		hasher 	hash;
		key_equal 	equals;
		ExtractKey 	get_key;
		vector<node*>	buckets;
		size_type 	ele_nums;

	public:
		hashtable(size_type n,const HashFcn& hfcn,const key_equal& kequal,const ExtractKey& ekey)
		:hash(hfcn),equals(kequal),get_key(ekey),buckets(),ele_nums(0)
		{
			initialize_buckets(n);
		}

		hashtable(size_type n,const HashFcn& hfcn,const key_equal& kequal)
		:hash(hfcn),equals(kequal),get_key(ExtractKey()),buckets(),ele_nums(0)
		{
			initialize_buckets(n);
		}

		hashtable(const hashtable& htable):hash(htable.hash),equals(htable.key_equal).get_key(htable.get_key),buckets(),ele_nums(htable.ele_nums)
		{
			buckets_copy_from(ht.buckets);
		}

		~hashtable() {clear();}

		size_type erase(const key_type& k)
		{
			const size_type index = bucket_num_by_key(k);
			node* first = buckets[index];
			size_type erased = 0;
			if(first)
			{
				node* cur = first;
				node* next = cur->next;
				while(next)
				{
					if(equals(get_key(next->val),k))
					{
						cur->next = next->next;
						delete_node(next);
						next = cur->next;
						++erased;
						--ele_nums;
					}
					else
					{
						cur = next;
						next = next->next;
					}
				}
				if (equals(get_key(first->val),k))
				{
					buckets[index] = first->next;
					delete_node(first);
					++erased;
					--ele_nums;
				}
			}
			return erased;
		}

		pair<iterator,bool> insert_unique(const value_type& v)
		{
			resize(ele_nums+1);
			return insert_unique_noresize(v);
		}

		iterator find(const key_type& k)
		{
			auto index = bucket_num_by_key(k);
			node* first = buckets[index];
			while(first!=nullptr && !equals(k,get_key(first->val)))
			{
				first = first->next;
			}
			return iterator(first,this);
		}

		const_iterator find(const key_type& k)const
		{
			auto index = bucket_num_by_key(k);
			node* first = buckets[index];
			while(first!=nullptr && !equals(k,get_key(first->val)))
			{
				first = first->next;
			}
			return const_iterator(first,this);
		}

		size_type size()const{return ele_nums;}
		bool empty()const{return ele_nums==0;}

		iterator begin()
		{
			for(auto index=0;index<buckets.size();++index)
			{
				if (buckets[index])
				{
					return iterator(buckets[index],this);
				}
			}
			return end();
		}

		const_iterator begin() const
		{
			for(auto index=0;index<buckets.size();++index)
			{
				if (buckets[index])
				{
					return const_iterator(buckets[index],this);
				}
			}
			return end();
		}

		iterator end() {return iterator(nullptr,this);}
		const_iterator end()const {return const_iterator(nullptr,this);}

	private:
		void initialize_buckets(size_type n)
		{
			const size_type num = next_prime(n);
			buckets.reserve(num);
			buckets.insert(buckets.end(),num,(node*)0);
		}
		size_type bucket_num_key(const key_type& k,size_type n)const
		{
			return hash(k)%n;
		}
		size_type bucket_num_val(const value_type& v,size_type n) const
		{
			return bucket_num_key(get_key(v),n);
		}
		size_type bucket_num_by_key(const key_type& k)const
		{
			return bucket_num_key(k,buckets.size());
		}

		size_type bucket_num_by_val(const value_type& v)const
		{
			return bucket_num_val(v,buckets.size());
		}

		void buckets_copy_from(const vector<node*>& buck)
		{
			buckets.clear();
			buckets.reserve(buck.size());
			buckets.insert(buckets.end(),buck.size(),(node*)0);
			for(auto i = 0;i<buck.size();++i)
			{
				if(const node* cur = buck[i])
				{
					node* copy = new_node(cur->val);
					buckets[i] = copy;
					for(node* next = cur->next;next!=nullptr;cur=next,next=cur->next)
					{
						copy->next = new_node(next->val);
						copy = copy->next;
					}
				}
			}
		}

		void clear()
		{
			for(size_type i = 0;i<buckets.size();++i)
			{
				node* cur = buckets[i];
				while(cur!=nullptr)
				{
					node* next = cur->next;
					delete_node(cur);
					cur = next;
				}
				buckets[i] = nullptr;
			}
			ele_nums = 0;
		}

		void resize(size_type num)
		{
			const size_type old_num = buckets.size();
			if (num>old_num)
			{
				const size_type n = next_prime(num);
				if (n>old_num)
				{
					vector<node*> temp(n,(node*)0);
					for(size_type index=0;index<old_num;++index)
					{
						node* first = buckets[index];
						while(first)
						{
							 size_type new_index = bucket_num_val(first->val,n);
							 buckets[index] = first->next;
							 first->next = temp[new_index];
							 temp[new_index] = first;
							 first = buckets[index];
						}
					}
					buckets.swap(temp);
				}
			}
		}

		pair<iterator,bool> insert_unique_noresize(const value_type& v)
		{
			const size_type index = bucket_num_by_val(v);
			node* first = buckets[index];
			for(node* cur = first;cur!=nullptr;cur=cur->next)
			{
				if (equals(get_key(v),get_key(cur->val)))
				{
					return pair<iterator,bool>(iterator(cur,this),false);
				}
			}
			node* temp = new_node(v);
			temp->next = buckets[index];
			buckets[index] = temp;
			++ele_nums;
			return pair<iterator,bool>(iterator(temp,this),true);
		}
	
	};

	//Iterator ++
	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>& hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>::operator++()
	{
		const node* old = cur;
		cur = cur->next;
		if(!cur)
		{
			size_type bucket = ht->bucket_num_by_val(old->val);
			while(!cur && ++bucket<ht->buckets.size())
			{
				cur = ht->buckets[bucket];
			}
		}
		return *this;
	}

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>::operator++(int)
	{
		iterator temp = *this;
		++(*this);
		return temp;
	}

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>& hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>::operator++()
	{
		const node* old = cur;
		cur = cur->next;
		if(!cur)
		{
			size_type bucket = ht->bucket_num_by_val(old->val);
			while(!cur && ++bucket<ht->buckets.size())
			{
				cur = ht->buckets[bucket];
			}
		}
		return *this;
	}

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey> hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>::operator++(int)
	{
		iterator temp = *this;
		++(*this);
		return temp;
	}

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	bool operator==(const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>& ht1,
					const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>& ht2)
	{
		typedef typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>::node node;
		if (ht1.buckets.size()!=ht2.buckets.size()||ht1.size()!=ht2.size())
		{
			return false;
		}
		for(auto index=0;index<ht1.buckets.size();++index)
		{
			node* cur1 = ht1.buckets[index];
			node* cur2 = ht2.buckets[index];
			while(cur1 && cur2 && cur1->val==cur2->val)
			{
				cur1 = cur1->next;
				cur2 = cur2->next;
			}
			if (cur1 || cur2)
			{
				return false;
			}
		}
		return true;
	}

	template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey>
	bool operator!=(const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>& ht1,
					const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey>& ht2)
	{
		return !(ht1==ht2);
	}
}
#endif