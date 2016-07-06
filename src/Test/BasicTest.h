#ifndef _MINI_STL_BASIC_H_
#define _MINI_STL_BASIC_H_


#include <cassert>
#include <vector>
#include <forward_list>
#include <iostream>
#include <algorithm>
#include "../Functional.h"
#include "../Heap.h"
#include "../Slist.h"
#include "../Vector.h"
#include "../RBTree.h"
#include "TestUtil.h"

namespace MINI_STL{
	namespace BasicTest{

		void heapTest();
		void slist_test();
		void rbtree_test();
		void testAllCases();
	}
}
#endif