#include"../util_tmp.h"
#include<vector>
#include<list>
#include<deque>
#include<stack>
#include<iostream>
int main()
{
//	std::list<std::vector<unsigned int> > test;
//	std::list<std::list<unsigned int> > test;
//	std::list<std::deque<unsigned int> > test;
	std::deque<std::vector<unsigned int> > test;
	std::list<std::deque<unsigned int> > test2;
//	std::vector<unsigned int> tmp={0,0,0};
//	tmp={1,2,3};
	test.push_back({1,2,3});
	test.push_back({4,5,6});
	test.push_back({7,8,9});
	std::cout << "min: " << get_min_val(test)<< "; max: " << get_max_val(test) << std::endl;
	test2.push_back({1,2,3});
	test2.push_back({4,5,6});
	test2.push_back({7,8,9});
	std::cout << "min: " << get_min_val(test2)<< "; max: " << get_max_val(test2) << std::endl;
	
}
