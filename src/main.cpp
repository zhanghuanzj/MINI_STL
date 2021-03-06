#include <iostream>
#include <cassert>

#include "Algorithm.h"
#include "Profiler\Profiler.h"
#include "Test\VectorTest.h"
#include "Test\ListTest.h"
#include "Test\DequeTest.h"
#include "Test\StackTest.h"
#include "Test\PriorityQueueTest.h"
#include <forward_list>
#include "Slist.h"
#include "Vector.h"
#include "Test\BasicTest.h" 
#include "Test\AlgorithmTest.h"

using namespace MINI_STL::Profiler;


int main(){
	//MINI_STL::AlgorithmTest::testAllCases();
	//MINI_STL::AVLTreeTest::testAllCases();
	//MINI_STL::BitmapTest::testAllCases();
	//MINI_STL::BinarySearchTreeTest::testAllCases();
	//MINI_STL::CircularBufferTest::testAllCases();
	//MINI_STL::COWPtrTest::testAllCases();
	MINI_STL::DequeTest::testAllCases();
	MINI_STL::ListTest::testAllCases();
	//MINI_STL::GraphTest::testAllCases();
	//MINI_STL::PairTest::testAllCases();
	MINI_STL::PriorityQueueTest::testAllCases();
	//MINI_STL::QueueTest::testAllCases();
	//MINI_STL::RefTest::testAllCases();
	//MINI_STL::SharedPtrTest::testAllCases();
	MINI_STL::StackTest::testAllCases();
	//MINI_STL::StringTest::testAllCases();
	//MINI_STL::SuffixArrayTest::testAllCases();
	//MINI_STL::TrieTreeTest::testAllCases();
	//MINI_STL::TypeTraitsTest::testAllCases();
	//MINI_STL::UFSetTest::testAllCases();
	//MINI_STL::UniquePtrTest::testAllCases();
	//MINI_STL::Unordered_setTest::testAllCases();
	MINI_STL::VectorTest::testAllCases();
	MINI_STL::BasicTest::testAllCases();
	MINI_STL::AlgorithmTest::testAllCases();
	system("pause");
	return 0;
}



