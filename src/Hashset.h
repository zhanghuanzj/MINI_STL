#ifndef _MINI_STL_HASHSET_H_
#define _MINI_STL_HASHSET_H_

#include "Hash_fcn.h"
#include "HashTable.h"
#include "Pair.h"
#include "Functional.h"
namespace MINI_STL
{
	template<class Value,class HashFcn=hash<Value>,class EqualKey=equal_to<Value>>
	class hashset;
	template <class Val, class HF, class EqK>  
  	inline bool operator== (const hashset<Val, HF, EqK>& hs1,const hashset<Val, HF, EqK>& hs2);
  	
	template<class Value,class HashFcn,class EqualKey>
	class hashset
	{
    private:
    	typedef hashtable<Value,Value,HashFcn,identity<Value>,EqualKey> ht;
    	ht htable;

    public:
    	typedef typename ht::key_type key_type;
    	typedef typename ht::value_type	value_type;
    	typedef typename ht::hasher hasher;
    	typedef typename ht::key_equal key_equal;

    	typedef typename ht::size_type size_type;
    	typedef typename ht::difference_type difference_type;
    	typedef typename ht::const_pointer pointer;
    	typedef typename ht::const_pointer const_pointer;
    	typedef typename ht::const_reference reference;
    	typedef typename ht::const_reference const_reference;

    	typedef typename ht::const_iterator iterator;
    	typedef typename ht::const_iterator const_iterator;
    	template <class Val, class HF, class EqK>  
  		friend bool operator== (const hashset<Val, HF, EqK>&,
                          		const hashset<Val, HF, EqK>&);

    	hashset():htable(100,hasher(),key_equal()){}

    	size_type size()const {return htable.size();}
    	bool empty()const {return htable.empty();}

    	iterator begin()const{return htable.begin();}
    	iterator end()const{return htable.end();}

    public:
    	pair<iterator,bool> insert(const value_type& v)
    	{
    		pair<typename ht::iterator,bool> p = htable.insert_unique(v);
    		return pair<iterator,bool>(p.first,p.second);
    	}

    	iterator find(const key_type& k)
    	{
    		return htable.find(k);
    	}

		const_iterator find(const key_type& k)const
		{
			return htable.find(k);
		}

    	size_type erase(const key_type& k)
    	{
    		return htable.erase(k);
    	}

		bool isContain(const key_type& k)const
		{
			return find(k)!=end();
		}

    	void clear()
    	{
    		htable.clear();
    	}
	};

	template <class Val, class HF, class EqK>  
  	inline bool operator== (const hashset<Val, HF, EqK>& hs1,const hashset<Val, HF, EqK>& hs2)
  	{
  		return hs1.htable==hs2.htable;
  	}
}
#endif