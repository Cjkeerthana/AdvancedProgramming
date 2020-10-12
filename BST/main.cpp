#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "bst.hpp"

int main(){
    try{
        std::cout << "*********************************" << std::endl;
        std::cout << "Binary Search Tree Implementation" << std::endl;
        std::cout << "*********************************" << std::endl;
        std::cout << "*********************************" << std::endl;
        std::cout << "*********************************" << std::endl;
        std::cout << std::endl;
        std::cout << "Implementation of the tree example given" << std::endl;

        Bst<int, int, std::less<int>> tree;

        std::cout << "Building the tree using inserting values into it" << std::endl;
        std::cout << "Inserting the values into the tree" << std::endl;
        tree.insert({8,8});
        tree.insert({3,3});
        tree.insert({6,6});
        tree.insert({1,1});
        tree.insert({10,10});
        tree.insert({7,7});
        tree.insert({14,14});
        tree.insert({4,4});
        tree.insert({13,13});
        std::cout << std::endl;
        std::cout << "Printing the tree" << std::endl;
        std::cout << "Tree :" << tree << std::endl;
        std::cout << std::endl;

        std::cout << "*********************************" << std::endl;
        std::cout << "Constructors: Copy and move" << std::endl;
        std::cout << "*********************************" << std::endl;
        std::cout << "Using the copy constructor" << std::endl;
        Bst<int, int, std::less<int>> tree1{tree};
        std::cout << "Copied tree :" << tree1 << std::endl;
        std::cout << std::endl;
        std::cout << "Inserting into the copied tree" << std::endl;
        tree1.insert({12,12});
        std::cout << "Copied tree :" << tree1 << std::endl;
        std::cout << "Original tree :" << tree << std::endl;
        std::cout << std::endl;

        std::cout << "Using copy assignment" << std::endl;
        Bst<int, int> tree2 = tree;
        std::cout << "Copied tree :" << tree2 << std::endl;
        std::cout << std::endl;

        std::cout << "Using the move constructor" << std::endl;
        Bst<int, int> tree3{std::move(tree)};
        tree.clear();
        std::cout << "Moved tree :" << tree3 << std::endl;
        std::cout << std::endl;

        std::cout << "Using move assignment" << std::endl;
        Bst<int, int> tree4 = tree2;
        std::cout << "Moved tree :" << tree4 << std::endl;
        std::cout << std::endl;

        std::cout << "*********************************" << std::endl;
        std::cout << "Tree operations " << std::endl;
        std::cout << "*********************************" << std::endl;
        std::cout << "1. EMPLACE" << std::endl;
        std::cout << "Using the emplace function for inserting" << std::endl;
        Bst<std::string, std::string> tree_string;
        tree_string.emplace("Keerthana", "Chandrasekar");
        tree_string.emplace("Azza", "Abdalghani");
        tree_string.emplace(std::make_pair("Giulia","Milano"));
        std::cout << "Insert using emplace :" << tree_string << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "2. BALANCE" << std::endl;
        std::cout << "A tree with greater than operator and unbalanced" << std::endl;
        Bst<int, int, std::greater<int>> tree_greater{7,7,std::greater<int>()};
        tree_greater.insert({1,1});
        tree_greater.insert({15,15});
        tree_greater.insert({3,3});
        tree_greater.insert({16,16});
        tree_greater.insert({18,18});
        tree_greater.insert({19,19});
        tree_greater.insert({38,38});
        tree_greater.insert({90,90});
        tree_greater.insert({123,123});
        tree_greater.insert({234,678});
        std::cout << "Unbalanced tree with greater than operator :" << std::endl;
        std::cout << tree_greater << std::endl;
        std::cout << std::endl;

        std::cout << "Balancing the greater tree" << std::endl;
        std::cout << "Is the tree balanced?" << std::endl;
        tree_greater.check_balance() ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
        tree_greater.balance();
        std::cout << std::endl;
        std::cout << "After Balancing " << std::endl;
        std::cout << "Balanced tree :" << tree_greater << std::endl;
        std::cout << std::endl;
        std::cout << "Is the tree now balanced?" << std::endl;
        tree_greater.check_balance() ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "3. ERASE" << std::endl;
        std::cout << "Erase the keys in a tree " << std::endl;
        std::cout << "Considering the given example tree " << std::endl;
        tree.insert({8,8});
        tree.insert({3,3});
        tree.insert({6,6});
        tree.insert({1,1});
        tree.insert({10,10});
        tree.insert({7,7});
        tree.insert({14,14});
        tree.insert({4,4});
        tree.insert({13,13});
        std::cout << tree << std::endl;
        std::cout << "Erasing a leaf node (4)" << std::endl;
        tree.erase(4);
        std::cout << tree << std::endl;
        std::cout << "Erasing a node lacking a left child (10)" << std::endl;
        tree.erase(10);
        std::cout << tree << std::endl;
        std::cout << "Erasing a node lacking a right child (14)" << std::endl;
        tree.erase(14);
        std::cout << tree << std::endl;
        std::cout << "Erasing a node having two children (3)" << std::endl;
        tree.erase(3);
        std::cout << tree << std::endl;
        std::cout << "Erasing the root with two children (8)" << std::endl;
        tree.erase(8);
        std::cout << tree << std::endl;
        std::cout << "Erasing the root lacking the right child (13)" << std::endl;
        tree.erase(13);
        std::cout << tree << std::endl;
        std::cout << "Erasing a leaf node (1)" << std::endl;
        tree.erase(1);
        std::cout << tree << std::endl;
        std::cout << "Erasing the root lacking the left child (6)" << std::endl;
        tree.erase(6);
        std::cout << tree << std::endl;
        std::cout << "Erase an node which is not in the tree (say 18)" << std::endl;
        tree.erase(18);
        std::cout << tree << std::endl;
        std::cout << std::endl;

        std::cout << "*********************************" << std::endl;
        std::cout << "Using the operator[]" << std::endl;
        Bst<double, double, std::less<double>> another_tree{9.1,9.1};
        another_tree[1.5] = 1.5;
        another_tree[3.4] = 3.4;
        another_tree[54.9] = 54.9;
        std::cout << "Another tree :" << another_tree << std::endl;
        std::cout << std::endl;
        std::cout << "*********************************" << std::endl;
        std::cout << "*********************************" << std::endl;
        //std::cout << "*********************************" << std::endl;
    }
    catch(const std::exception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << "Unknown exception occured" << std::endl;
        return 2;
    }
	return 0;
}