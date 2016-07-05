#ifndef _MINI_STL_QUEUE_H_
#define _MINI_STL_QUEUE_H_

#include "Deque.h"

namespace MINI_STL
{

	template<class T,class Sequence=deque<T>>
	class queue;

	template<class T,class Sequence>
	bool operator==(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2)
	{
		return q1.seq == q2.seq;
	}

	template<class T,class Sequence>
	bool operator!=(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2)
	{
		return !(q1.seq == q2.seq);
	}

	template<class T,class Sequence>
	bool operator<(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2)
	{
		return q1.seq < q2.seq;
	}

	template<class T,class Sequence>
	bool operator>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2)
	{
		return q2.seq<q1.seq;
	}

	template<class T,class Sequence>
	bool operator<=(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2)
	{
		return !(q1.seq > q2.seq);
	}

	template<class T,class Sequence>
	bool operator>=(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2)
	{
		return !(q1.seq < q2.seq);
	}

	template<class T,class Sequence>
	class queue
	{
		friend bool operator== <T,Sequence>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2);
		friend bool operator>= <T,Sequence>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2);
		friend bool operator<= <T,Sequence>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2);
		friend bool operator<  <T,Sequence>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2);
		friend bool operator>  <T,Sequence>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2);
		friend bool operator!= <T,Sequence>(const queue<T,Sequence>& q1,const queue<T,Sequence>& q2);
	public:
      	typedef typename Sequence::value_type value_type;
      	typedef typename Sequence::size_type size_type;
      	typedef typename Sequence::reference reference;
      	typedef typename Sequence::const_reference const_reference;


  	private:
  		Sequence seq;

  	public:
  		queue(){}
  		explicit queue(const Sequence& s):seq(s){};

  		bool empty() const{ return seq.empty();}
  		size_type size()const {return seq.size();}
  		reference front() {return seq.front();}
  		const_reference front()const{return seq.front();}
  		reference back() {return seq.back();}
  		const_reference back()const {return seq.back();}
  		void push(const value_type& x){seq.push_back(x);}
  		void pop(){seq.pop_front();}

		void swap(queue& x){seq.swap(x.seq);}

	};
}
#endif