#include "BasicTest.h"

namespace MINI_STL
{
	namespace BasicTest{
		
		void heap_test()
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

			std::cout<<"Heap test ok!"<<std::endl;
		}


		void slist_test()
		{
			MINI_STL::slist<int> sl;
			std::forward_list<int> fl;
			assert(sl.size()==0);

			sl.push_front(9);
			sl.push_front(1);
			sl.push_front(2);
			sl.push_front(3);
			sl.push_front(4);

			fl.push_front(9);
			fl.push_front(1);
			fl.push_front(2);
			fl.push_front(3);
			fl.push_front(4);
			assert(sl.size()==5);
			assert(MINI_STL::Test::container_equal(sl,fl));



			auto it = find(sl.begin(),sl.end(),1);
			auto fit = MINI_STL::find(fl.begin(),fl.end(),2);
			if (it!=nullptr)
			{
				sl.insert(it,99);
				fl.insert_after(fit,99);
			}
			assert(sl.size()==6);
			assert(*it==1);
			assert(MINI_STL::Test::container_equal(sl,fl));

			it = find(sl.begin(),sl.end(),3);
			fit =  MINI_STL::find(fl.begin(),fl.end(),4);
			if (it!=nullptr)
			{
				assert(*(sl.erase(it))==2);
				fl.erase_after(fit);
			}
			assert(MINI_STL::Test::container_equal(sl,fl));
			std::cout<<"SList test ok!"<<std::endl;
		}

		void rbtree_test()
		{
			RBTree<int,int,MINI_STL::identity<int>,MINI_STL::less<int>> tree;
			assert(tree.size()==0);
			tree.insert_unique(10);
			tree.insert_unique(7);
			tree.insert_unique(8);

			tree.insert_unique(15);
			tree.insert_unique(5);
			tree.insert_unique(6);

			tree.insert_unique(11);

			tree.insert_unique(13);
			tree.insert_unique(12);

			RBTree<int,int,MINI_STL::identity<int>,MINI_STL::less<int>> tree2;
			tree2.insert_unique(tree.begin(),tree.end());
			assert(tree.size()==9);
			assert(tree2.size()==9);
			int a[9] = {5,6,7,8,10,11,12,13,15};
			assert(MINI_STL::Test::container_equal(a,tree2));
			assert(MINI_STL::Test::container_equal(a,tree));

			
			//auto i1 = tree.begin();
			//auto i2 = tree.end();
			//while (i1!=i2)
			//{
			//	std::cout<<*i1<<" ";
			//	++i1;
			//}
			//std::cout<<std::endl;

			bool b[9] = {false,true,false,true,true,false,false,true,false};
			bool result[9] ;

			int index = 0;
			auto it1 = tree.begin();
			auto it2 = tree.end();
			while (it1!=it2)
			{
				//std::cout<<*it1<<"("<<RBTree_iterator_base(it1).node->color<<")"<<" ";
				result[index++] = RBTree_iterator_base(it1).node->color;
				++it1;
			}
			assert(MINI_STL::Test::container_equal(b,result));

			tree.erase(8);
			assert(tree.size()==8);

			std::cout<<"RBTree test ok!"<<std::endl;
		}

		void set_test()
		{
			int a[5] = {0,1,2,3,4};
			std::set<int> s1(a,a+5);
			MINI_STL::set<int> s2(a,a+5);
			assert(MINI_STL::Test::container_equal(s1,s2));

			s1.insert(3);
			s2.insert(3);
			s1.insert(5);
			s2.insert(5);
			assert(MINI_STL::Test::container_equal(s1,s2));
			s1.erase(1);
			s2.erase(1);
			assert(MINI_STL::Test::container_equal(s1,s2));
			assert(s1.size()==s2.size());
			assert(s1.count(3)==s2.count(3));

			auto i1 = s1.find(3);
			auto i2 = s2.find(3);
			assert(*i1==*i2);

			i1 = s1.find(1);
			i2 = s2.find(1);
			assert(i1==s1.end()&&i2==s2.end());
			std::cout<<"Set test ok!"<<std::endl;
		}

		void multiset_test()
		{
			int a[5] = {0,1,2,3,4};
			std::multiset<int> s1(a,a+5);
			MINI_STL::multiset<int> s2(a,a+5);
			assert(MINI_STL::Test::container_equal(s1,s2));

			s1.insert(3);
			s2.insert(3);
			s1.insert(5);
			s2.insert(5);
			s1.insert(3);
			s2.insert(3);
			s1.insert(5);
			s2.insert(5);
			assert(MINI_STL::Test::container_equal(s1,s2));
			s1.erase(1);
			s2.erase(1);
			assert(MINI_STL::Test::container_equal(s1,s2));
			assert(s1.size()==s2.size());
			assert(s1.count(3)==s2.count(3));

			s1.erase(3);
			s2.erase(3);
			assert(MINI_STL::Test::container_equal(s1,s2));
			assert(s1.size()==s2.size());
			assert(s1.count(3)==s2.count(3));
			auto i1 = s1.find(5);
			auto i2 = s2.find(5);
			assert(*i1==*i2);

			i1 = s1.find(3);
			i2 = s2.find(3);
			assert(i1==s1.end()&&i2==s2.end());
			std::cout<<"Multiset test ok!"<<std::endl;
		}

		void map_test()
		{
			std::map<std::string,int> m1;
			MINI_STL::map<std::string,int> m2;
			m1[std::string("one")] = 1;
			m1[std::string("two")] = 2;
			m1[std::string("three")] = 3;
			m1[std::string("four")] = 4;

			m2[std::string("one")] = 1;
			m2[std::string("two")] = 2;
			m2[std::string("three")] = 3;
			m2[std::string("four")] = 4;
			assert(MINI_STL::Test::map_equal(m1,m2));
			
			m1.insert(make_pair(std::string("five"),5));
			m2.insert(MINI_STL::make_pair(std::string("five"),5));
			assert(MINI_STL::Test::map_equal(m1,m2));
			assert(m1[std::string("three")]==m2[std::string("three")]);

			m1[std::string("three")] = 8;
			m2[std::string("three")] = 8;
			assert(MINI_STL::Test::map_equal(m1,m2));

			auto it1 = m1.find(std::string("three"));
			auto it2 = m2.find(std::string("three"));
			assert((*it1).second == (*it2).second );
			std::cout<<"Map test ok!"<<std::endl;
		}

		void multimap_test()
		{
			std::multimap<std::string,int> m1;
			MINI_STL::multimap<std::string,int> m2;
			m1.insert(make_pair(std::string("one"),1));
			m1.insert(make_pair(std::string("two"),2));
			m1.insert(make_pair(std::string("three"),3));
			m1.insert(make_pair(std::string("four"),4));
			m1.insert(make_pair(std::string("three"),3));
			m1.insert(make_pair(std::string("four"),4));

			m2.insert(MINI_STL::make_pair(std::string("one"),1));
			m2.insert(MINI_STL::make_pair(std::string("two"),2));
			m2.insert(MINI_STL::make_pair(std::string("three"),3));
			m2.insert(MINI_STL::make_pair(std::string("four"),4));
			m2.insert(MINI_STL::make_pair(std::string("three"),3));
			m2.insert(MINI_STL::make_pair(std::string("four"),4));

			assert(MINI_STL::Test::map_equal(m1,m2));

			m1.insert(make_pair(std::string("five"),5));
			m2.insert(MINI_STL::make_pair(std::string("five"),5));
			assert(MINI_STL::Test::map_equal(m1,m2));

			auto it1 = m1.find(std::string("three"));
			auto it2 = m2.find(std::string("three"));
			assert((*it1).second == (*it2).second );
			std::cout<<"Multimap test ok!"<<std::endl;
		}

		void hashtable_test()
		{
			MINI_STL::hashtable<int,int,MINI_STL::hash<int>,identity<int>,std::equal_to<int>> ht(50,MINI_STL::hash<int>(),std::equal_to<int>());
			std::cout<<ht.size();
			std::cout<<"Hashtable test ok!"<<std::endl;
		}
		void testAllCases()
		{
			rbtree_test();
			heap_test();
			slist_test();	
			set_test();
			multiset_test();
			map_test();
			multimap_test();
			hashtable_test();
		}
	}
}
