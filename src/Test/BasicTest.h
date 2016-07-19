#ifndef _MINI_STL_BASIC_H_
#define _MINI_STL_BASIC_H_


#include <cassert>
#include <vector>
#include <forward_list>
#include <iostream>
#include <algorithm>
#include <set>
#include <string>
#include <map>
#include "../Functional.h"
#include "../Heap.h"
#include "../Slist.h"
#include "../Vector.h"
#include "../RBTree.h"
#include "../Set.h"
#include "../Multiset.h"
#include "../Map.h"
#include "../Multimap.h"
#include "../HashTable.h"
#include "../Hash_fcn.h"
#include "TestUtil.h"

namespace MINI_STL{
	namespace BasicTest{

		void heap_test();
		void slist_test();
		void rbtree_test();
		void set_test();
		void multiset_test();
		void map_test();
		void multimap_test();
		void hashtable_test();
		void testAllCases();

	}
}
#endif