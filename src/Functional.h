#ifndef _MINI_STL_FUNCTIONAL_H_
#define _MINI_STL_FUNCTIONAL_H_ 

namespace MINI_STL
{
	template<class T>
	struct less
	{
		bool operator()(const T& x,const T& y)const
		{
			return x<y;
		}
	};

	template<class T>
	struct identity
	{
		const T& operator()(const T& x)const
		{
			return x;
		}
	};
}
#endif