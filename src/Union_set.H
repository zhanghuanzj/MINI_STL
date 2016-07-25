#ifndef _MINI_STL_UNION_SET_H_
#define _MINI_STL_UNION_SET_H_
#include "Vector.h"
namespace MINI_STL
{
	class Union_set
	{
	private:
		vector<int> scale;
		vector<int> tree;
		int count;         //连通分量
	public:
		Union_set(int n):count(n)
		{
			scale.resize(n+1,1);
			tree.reserve(n+1);
			for(int i=0;i<=n;++i)
				tree.push_back(i);
		}
		
		int find(int id)const
		{
			while(id!=tree[id])
				id = tree[id];
			return id;
		}
	    bool isConnected(int x,int y)const
	    {
	    	return find(x)==find(y);
	    }

		void unionSet(int x,int y)
		{
			int idx = find(x);
			int idy = find(y);
			if (idx==idy)
				return;
			if(scale[idx]<scale[idy])
			{
                scale[idy] += scale[idx];
                tree[idx] = tree[idy];
			}
			else
			{
				scale[idx] += scale[idy];
				tree[idy] = tree[idx];
			}
			--count;
		}

		int getCount()const
		{
			return count;
		}
	};
}
#endif