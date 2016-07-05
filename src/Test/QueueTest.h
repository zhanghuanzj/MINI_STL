#ifndef _MINI_STL_QUEUE_TEST_H_
#define _MINI_STL_QUEUE_TEST_H_

#include "TestUtil.h"

#include "../Queue.h"
#include <queue>

#include <cassert>
#include <string>

namespace MINI_STL{
	namespace QueueTest{
#define  stdQ  std::queue 
#define  tsQ  MINI_STL::queue

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();

		void testAllCases();
	}
}

#endif