#ifndef _MINI_STL_VECTOR_H_
#define _MINI_STL_VECTOR_H_

#include "Algorithm.h"
#include "Allocator.h"
#include "Construct.h"
#include "Uninitialized.h"
#include "ReverseIterator.h"

namespace MINI_STL
{
	template<class T,class Alloc=Allocator<T>>
	class vector
	{
	public:
		typedef T								value_type;
		typedef T*								pointer;
		typedef T*								iterator;
		typedef reverse_iterator_t<T*>			reverse_iterator;
		typedef reverse_iterator_t<const T*>	const_reverse_iterator;
		typedef const T*						const_iterator;
		typedef T&								reference;
		typedef const T&						const_reference;
		typedef size_t							size_type;
		typedef ptrdiff_t						difference_type;
		typedef Alloc							dataAllocator;
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;

		//内存不够时进行插入
		void insert_aux(iterator position,const T& x);
		//释放内存
		void deallocate()
		{
			if (start)
			{
				dataAllocator::deallocate(start,end_of_storage-start);
			}
		}
		//分配内存并填入数据
		iterator allocate_and_fill(size_type n,const T &x)
		{
			iterator dest = dataAllocator::allocate(n);
			Uninitialized_fill_n(dest,n,x);
			return dest;
		}
		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last){
			start = dataAllocator::allocate(last - first);
			finish = Uninitialized_copy(first, last, start);
			end_of_storage = finish;
		}
		template<class InputIterator>
		iterator allocate_and_copy(size_type n,InputIterator first, InputIterator last){
			iterator result = dataAllocator::allocate(n);
			Uninitialized_copy(first, last, result);
			return result;
		}
		//分配内存并填入数据&更新空间状态
		void fill_initialize(size_type n,const T& value)
		{
			start = allocate_and_fill(n,value);
			finish = start+n;
			end_of_storage = finish;
		}

	public:
		//构造函数
		vector():start(0),finish(0),end_of_storage(0){}
		vector(const vector& v):vector(v.begin(), v.end()){}
		vector(size_type n,const T& value){fill_initialize(n,value);}
		vector(int n,const T& value){fill_initialize(n,value);}
		vector(long n,const T& value){fill_initialize(n,value);}
		explicit vector(size_type n){fill_initialize(n,T());}
		template<class InputIterator>
		vector(InputIterator first,InputIterator last){allocate_and_copy(first,last);}
		//析构函数
		~vector()
		{
			destroy(start,finish);
			deallocate();
		}

		//非更易型操作
		bool empty() const {return begin()==end();}
		size_type size() const {return size_type(finish-start);}
		size_type capacity() const {return size_type(end_of_storage-start);}
		void reserve(size_type n)
		{
			if (capacity()<n)
			{
				const size_type old_size = size();
				iterator temp = allocate_and_copy(n,start,finish);
				destroy(start,finish);
				deallocate();
				start = temp;
				finish = temp+old_size;
				end_of_storage = temp+n;
			}
		}
		void shrink_to_fit()
		{
			iterator new_start = dataAllocator::allocate(size());
			finish = Uninitialized_copy(start,finish,new_start);
			deallocate();
			start = new_start;
			end_of_storage = finish;
		}
		//赋值
		vector<T,Alloc>& operator=(const vector<T,Alloc>& v);
		vector<T,Alloc>& operator=(vector<T,Alloc>&& v);
		void swap(vector &v);
		//元素访问
		reference operator[](const difference_type n){return *(begin()+n);}
		const_reference operator[](const difference_type n)const{return *(cbegin()+n);}
		reference front(){return *begin();}
		reference back(){return *(end()-1);}
		pointer data(){ return start; }

		//迭代器相关
		iterator begin(){return start;}
		iterator end(){return finish;}
		const_iterator begin()const{return start;}
		const_iterator end()const{return finish;}
		const_iterator cbegin()const{return start;};
		const_iterator cend()const{return finish;}
		reverse_iterator rbegin(){return reverse_iterator(finish);}
		reverse_iterator rend(){return reverse_iterator(start);}
		const_reverse_iterator crbegin()const{return const_reverse_iterator(finish);}
		const_reverse_iterator crend()const{return const_reverse_iterator(start);}

		//插入删除
		void pop_back()
		{
			--finish;               //当vector为空时会出错
			destroy(finish);
		}

		void push_back(const T& x)
		{
			if (finish!=end_of_storage)
			{
				construct(finish,x);
				++finish;
			}
			else
			{
				insert_aux(end(),x);
			}
		}

		void insert(iterator position,size_type n,const T& x);
		void insert(iterator position,const T& x);
		//template <class InputIterator>
		void insert(iterator position,iterator first,iterator last)
		{
			if (first != last) {
				size_type n = last-first;
				if (size_type(end_of_storage - finish) >= n) {
					const size_type elems_after = finish - position;
					iterator old_finish = finish;
					if (elems_after > n) {
						Uninitialized_copy(finish - n, finish, finish);
						finish += n;
						copy_backward(position, old_finish - n, old_finish);
						copy(first, last, position);
					}
					else {
						Uninitialized_copy(first + elems_after, last, finish);
						finish += n - elems_after;
						Uninitialized_copy(position, old_finish, finish);
						finish += elems_after;
						copy(first, first + elems_after, position);
					}
				}
				else {
					const size_type old_size = size();
					const size_type len = old_size + max(old_size, n);
					iterator new_start = dataAllocator::allocate(len);
					iterator new_finish = new_start;
					new_finish = Uninitialized_copy(start, position, new_start);
					new_finish = Uninitialized_copy(first, last, new_finish);
					new_finish = Uninitialized_copy(position, finish, new_finish);
					
					destroy(start, finish);
					deallocate();
					start = new_start;
					finish = new_finish;
					end_of_storage = new_start + len;
				}
			}
		}

		iterator erase(iterator position)
		{
			if (position+1!=end())  //后续元素往前移，后面的进行析构
			{
				copy(position+1,finish,position);
			}
			--finish;
			destroy(finish);
			return position;
		}

		iterator erase(iterator first,iterator last)
		{
			iterator i = copy(last,finish,first);
			destroy(i,finish);
			finish = finish-(last-first);
			return first;
		}

		void resize(size_type new_size,const T&x)
		{
			if (new_size<size())    //将多的析构
			{
				erase(begin()+new_size,end());
			}
			else
			{
				insert(end(),new_size-size(),x);
			}
		}

		void resize(size_type new_size)
		{
			resize(new_size,T());
		}

		void clear()             //元素全部析构
		{
			erase(begin(),end());
		}
	};

	//赋值
	template<class T,class Alloc>
	vector<T,Alloc>& vector<T,Alloc>::operator=(const vector<T,Alloc>& v)
	{
		if (this!=&v)
		{
			const size_type len= v.size();
			if (len>capacity())
			{
				iterator temp = allocate_and_copy(len,v.begin(),v.end());
				destroy(start,finish);
				deallocate();
				start = temp;
				finish = temp+len;
				end_of_storage = finish;
			}
			else if (len<size())
			{
				iterator temp = copy(v.begin(),v.end(),start);
				destroy(temp,finish);
				finish = start+len;
			}
			else
			{
				copy(v.begin(),v.begin()+size(),start);
				finish = Uninitialized_copy(v.begin()+size(),v.end(),finish);
			}
		}
		return *this;
	}

	template<class T,class Alloc>
	vector<T,Alloc>& vector<T,Alloc>::operator=(vector<T,Alloc>&& v)
	{
		if (this!=&v)
		{
			destroy(start,finish);
			deallocate();
			start = v.start;
			finish = v.finish;
			end_of_storage = v.end_of_storage;
			v.start = v.finish = v.end_of_storage = 0;
		}
		return *this;
	}

	template<class T,class Alloc>
	void vector<T,Alloc>::swap(vector<T,Alloc>& v)
	{
		if (this!=&v)
		{
			MINI_STL::swap(start,v.start);
			MINI_STL::swap(finish,v.finish);
			MINI_STL::swap(end_of_storage,v.end_of_storage);
		}
	}
	//非更易型操作
	template<class T,class Alloc>
	inline bool operator==(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
	{
		return v1.size()==v2.size()&&equal(v1.begin(),v1.end(),v2.begin());
	}

	template<class T,class Alloc>
	inline bool operator!=(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
	{
		return !(v1==v2);
	}

	template<class T,class Alloc>
	inline bool operator<(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
	{
		return lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end());
	}

	template<class T,class Alloc>
	inline bool operator>(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
	{
		return v2<v1;
	}

	template<class T,class Alloc>
	inline bool operator<=(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
	{
		return !(v1>v2);
	}

	template<class T,class Alloc>
	inline bool operator>=(const vector<T,Alloc>& v1,const vector<T,Alloc>& v2)
	{
		return !(v1<v2);
	}

	template<class T,class Alloc>
	void vector<T,Alloc>::insert(iterator position,size_type n,const T& x)
	{
		if (n!=0)
		{
			//1.剩余空间可以容下所要插入的元素
			if (size_type(end_of_storage-finish)>=n)
			{
				T copy_x = x;  //使之可以传给函数
				const size_type elems_after = finish-position;
				iterator old_finish = finish;
				if (elems_after>n)
				{
					Uninitialized_copy(finish-n,finish,finish);
					finish = finish+n;
					copy_backward(position,old_finish-n,old_finish);
					fill(position,position+n,copy_x);
				}
				else 
				{
					finish = Uninitialized_fill_n(finish,n-elems_after,copy_x);
					finish = Uninitialized_copy(position,old_finish,finish);
					fill(position,old_finish,copy_x);
				}
			}
			//2.剩余空间容不下所要插入的元素
			else
			{
				const size_type old_size = size();
				const size_type new_size = old_size+max(old_size,n);

				iterator new_start = dataAllocator::allocate(new_size);
				iterator new_finish = new_start;
				new_finish = Uninitialized_copy(start,position,new_start);
				new_finish = Uninitialized_fill_n(new_finish,n,x);
				new_finish = Uninitialized_copy(position,finish,new_finish);

				destroy(start,finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = start+new_size;
			}
		}
	}

	template<class T,class Alloc>
	void vector<T,Alloc>::insert(iterator position,const T& x)
	{
		insert(position,1,x);
	}

	template<class T,class Alloc>
	void vector<T,Alloc>::insert_aux(iterator position,const T& x)
	{
		if (finish!=end_of_storage)
		{
			construct(finish,*(finish-1));
			++finish;
			MINI_STL::copy_backward(position,finish-2,finish-1);
			*position = x;
		}
		else
		{
			const size_type old_size = size();
			const size_type new_size = old_size==0?1:2*old_size;

			iterator new_start = dataAllocator::allocate(new_size);
			iterator new_finish = new_start;
			new_finish = Uninitialized_copy(start,position,new_start);
			construct(new_finish,x);
			++new_finish;
			new_finish = Uninitialized_copy(position,finish,new_finish);

			destroy(begin(),end());
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start+new_size;
		}
	}
}
#endif // !_MINI_STL_VECTOR_H_

