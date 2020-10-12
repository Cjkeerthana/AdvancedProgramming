#ifndef __iterator_hpp
#define __iterator_hpp

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "bst.hpp"

/*
************* Class Iterator for the BST *****************
A forward iterator has been used for our container
*/
template <typename node_type, typename O>
    class __iterator{
		node_type* current;

		public:
			explicit __iterator(node_type* x) noexcept: current{x} {}

			node_type* getCurrent() const { return current; }

			friend bool operator==(const __iterator&a, const __iterator&b) { return a.current == b.current; } //equality of the nodes
			friend bool operator!=(const __iterator&a, const __iterator&b) { return !(a == b); }
			
			using val_type = O;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			using reference = val_type&;
			using pointer = val_type*;

			reference operator*() const noexcept { return current->value; }
			pointer operator->() const noexcept { return &(*(*this)); }

			//The inorder traversal of the tree is done by overloading the pre increment operator++
			__iterator& operator++() noexcept{			
				if(current->right){							//The sucessor of each node is found 
					current = current->right.get();			//if the node has a right branch 
					while(current->left)					//go to the leftmost node of the right branch
						current = current->left.get();
				}
				else if(!current->parent){					//If node is the root, set current to nullptr
					current = nullptr;
				}else if(current->parent->left.get() == current){	//If the node is a left child
					current = current->parent;						//then the succesor is the parent itself 
				}else{												
					while (current->parent && current->parent->right.get() == current)
						current = current->parent;					//Else when the node is a right child, keep traversing
					current = current->parent;						//until the node ceases to be a right child or
				}													//becomes the root. The successor is the parent of									
				return *this;										//of this node.
			}

			__iterator& operator++(int) noexcept{
				__iterator tmp{current};
				++(*this);
				return tmp;
			}
};

#endif