#ifndef _PRIORITY_QUEUE_TEST_H_
#define _PRIORITY_QUEUE_TEST_H_

#include "TestUtil.h"

#include "../Priority_queue.h"
#include <queue>
#include <algorithm>
#include <cassert>
#include <string>

namespace MINI_STL{
	namespace PriorityQueueTest{
#define  stdPQ  std::priority_queue
#define  tsPQ  MINI_STL::priority_queue 

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();

		void testAllCases();
	}
}

#endif