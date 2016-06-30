#ifndef _MINI_STL_FUNCTIONAL_H_
#define _MINI_STL_FUNCTIONAL_H_ 

namespace MINI_STL
{
	template<class T>
	struct less
	{
		bool operator()(const T& x,const T& y)
		{
			return x<y;
		}
	};
}
#endif