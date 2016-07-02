#ifndef _DEQUE_TEST_H_
#define _DEQUE_TEST_H_

#include "TestUtil.h"

#include "../Deque.h"
#include <deque>

#include <cassert>
#include <string>

namespace MINI_STL{
	namespace DequeTest{

#define  stdDQ std::deque 
#define  tsDQ MINI_STL::deque

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();
		void testCase5();
		void testCase6();

		void testAllCases();
	}
}

#endif