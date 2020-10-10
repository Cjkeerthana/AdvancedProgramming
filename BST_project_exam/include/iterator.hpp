#ifndef __iterator_hpp
#define __iterator_hpp

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "bst.hpp"

template <typename node_type, typename O>
    class __iterator{
		node_type* current;

		public:
			explicit __iterator(node_type* x) noexcept: current{x} {}

			node_type* getCurrent() const { return current; }

			friend bool operator==(const __iterator&a, const __iterator&b) { return a.current == b.current; }
			friend bool operator!=(const __iterator&a, const __iterator&b) { return !(a == b); }
			
			using val_type = O;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			using reference = val_type&;
			using pointer = val_type*;

			reference operator*() const noexcept { return current->value; }
			pointer operator->() const noexcept { return &(*(*this)); }

			__iterator& operator++() noexcept{			
				if(current->right){
					current = current->right.get();
					while(current->left)
						current = current->left.get();
				}
				else if(!current->parent){
					current = nullptr;
				}else if(current->parent->left.get() == current){
					current = current->parent;
				}else{
					while (current->parent && current->parent->right.get() == current)
						current = current->parent;
					current = current->parent;
				}
				
				return *this;
			}

			__iterator& operator++(int) noexcept{
				__iterator tmp{current};
				++(*this);
				return tmp;
			}
};

#endif