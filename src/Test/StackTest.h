#ifndef _STACK_TEST_H_
#define _STACK_TEST_H_

#include "TestUtil.h"

#include "../Stack.h"
#include <stack>

#include <cassert>
#include <string>

namespace MINI_STL{
	namespace StackTest{
#define  stdSt std::stack
#define	 tsSt  MINI_STL::stack 

		void testCase1();
		void testCase2();
		void testCase3();
		void testCase4();

		void testAllCases();
	}
}

#endif