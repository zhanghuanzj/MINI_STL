#include "AlgorithmTest.h"

namespace MINI_STL{
	namespace AlgorithmTest{
		void accumulate_test()
		{
			int a[] = {1,2,3,4,5,6,7,8,9,0};
			vector<int> v(a,a+10);
			assert(MINI_STL::accumulate(v.begin(),v.end(),0)==45);
			assert(MINI_STL::accumulate(v.begin(),v.end(),0,std::minus<int>())==-45);
		}

		void testAllCases()
		{
			accumulate_test();
		}
	}
}