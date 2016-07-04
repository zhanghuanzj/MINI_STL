#ifndef _MINI_STL_STACK_H_
#define _MINI_STL_STACK_H_

#include "Deque.h"
namespace MINI_STL
{
	template<class T,class Sequence=deque<T>>
	class stack;

	template<class T,class Sequence>
	bool operator==(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2)
	{
		return s1.seq == s2.seq;
	}

	template<class T,class Sequence>
	bool operator!=(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2)
	{
		return !(s1.seq == s2.seq);
	}

	template<class T,class Sequence>
	bool operator<(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2)
	{
		return s1.seq < s2.seq;
	}

	template<class T,class Sequence>
	bool operator>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2)
	{
		return s2.seq<s1.seq;
	}

	template<class T,class Sequence>
	bool operator<=(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2)
	{
		return !(s1.seq > s2.seq);
	}

	template<class T,class Sequence>
	bool operator>=(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2)
	{
		return !(s1.seq < s2.seq);
	}

	template<class T,class Sequence>
	class stack
	{
		friend bool operator== <T,Sequence>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2);
		friend bool operator>= <T,Sequence>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2);
		friend bool operator<= <T,Sequence>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2);
		friend bool operator<  <T,Sequence>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2);
		friend bool operator>  <T,Sequence>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2);
		friend bool operator!= <T,Sequence>(const stack<T,Sequence>& s1,const stack<T,Sequence>& s2);
	public:
      	typedef typename Sequence::value_type value_type;
      	typedef typename Sequence::size_type size_type;
      	typedef typename Sequence::reference reference;
      	typedef typename Sequence::const_reference const_reference;


  	private:
  		Sequence seq;

  	public:
  		stack(){}
  		explicit stack(const Sequence& s):seq(s){} 

  		bool empty() {return seq.empty();}
  		size_type size() {return seq.size();}
  		reference top() {return seq.back();}
  		const_reference top()const {return seq.back();}
  		void push(const value_type& x) {seq.push_back(x);}
  		void pop() {seq.pop_back();}
		void swap(stack& x)
		{
			seq.swap(x.seq);
		}
	};


}


#endif