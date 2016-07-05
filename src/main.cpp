#include <iostream>
#include <cassert>

#include "Algorithm.h"
#include "Profiler\Profiler.h"
#include "Test\VectorTest.h"
#include "Test\ListTest.h"
#include "Test\DequeTest.h"
#include "Test\StackTest.h"
#include "Test\PriorityQueueTest.h"
#include "Heap.h"
using namespace MINI_STL::Profiler;

void heapTest();
int main(){
	heapTest();
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
	
	system("pause");
	return 0;
}

void heapTest()
{
	int ia[9] = {0,1,2,3,4,8,9,3,5};
	MINI_STL::vector<int> vec(ia,ia+9);
	std::vector<int> vecTest(ia,ia+9);
	MINI_STL::make_heap(vec.begin(),vec.end());
	std::make_heap(vecTest.begin(),vecTest.end());
	assert(MINI_STL::Test::container_equal(vec, vecTest));

	vec.push_back(7);
	vecTest.push_back(7);
	MINI_STL::push_heap(vec.begin(),vec.end());
	std::push_heap(vecTest.begin(),vecTest.end());
	assert(MINI_STL::Test::container_equal(vec, vecTest));

	MINI_STL::pop_heap(vec.begin(),vec.end());
	std::pop_heap(vecTest.begin(),vecTest.end());
	vec.pop_back();
	vecTest.pop_back();
	assert(MINI_STL::Test::container_equal(vec, vecTest));

	MINI_STL::sort_heap(vec.begin(),vec.end());
	std::sort_heap(vecTest.begin(),vecTest.end());
	assert(MINI_STL::Test::container_equal(vec, vecTest));
}