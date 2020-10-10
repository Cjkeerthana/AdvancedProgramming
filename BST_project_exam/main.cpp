#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "bst.hpp"
#include "iterator.hpp"

int main(){
	Bst<int, int, std::less<int>> tree;
    tree.insert({8,8});
    tree.insert({3,3});
    tree.insert({6,6});
    tree.insert({1,1});
    tree.insert({10,10});
    tree.insert({7,7});
	tree.insert({14,14});
    tree.insert({4,4});
    tree.insert({13,13});
    tree.insert({5,5});
	std::cout << tree << std::endl;
	
	tree.erase(3);
	
    std::cout << tree << std::endl;
    //std::cout << tree << std::endl;
	//tree.clear();
	return 0;
}
