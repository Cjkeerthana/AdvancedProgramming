#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "bst.hpp"
#include "iterator.hpp"

//BST OPERATIONS 

template <typename key_type, typename value_type, typename comp_op>
	std::pair<typename Bst<key_type, value_type, comp_op>::iterator, bool> Bst<key_type, value_type, comp_op>::insert(const pair_type& x){
		node_type* tmp = root.get();
		if(!tmp){
			root = std::make_unique<node_type>(x);
			return std::make_pair<iterator,bool>(iterator(root.get()), true);
		}
		node_type* new_node = new node_type(x);
		while (tmp)
		{
			if(compare(x.first,tmp->value.first)){
				if(!tmp->left){
					new_node->parent = tmp;
					tmp->left.reset(new_node);
					return std::make_pair<iterator,bool>(iterator(new_node), true);
				}
				tmp = tmp->left.get();
			}
			else if (compare(tmp->value.first, x.first)){
				if(!tmp->right){
					new_node->parent = tmp;
					tmp->right.reset(new_node);
					return std::make_pair<iterator,bool>(iterator(new_node), true);
				}
				tmp = tmp->right.get();
			}
			else 
				return std::make_pair<iterator,bool>(iterator(tmp), false);
		}
		
}


template <typename key_type, typename value_type, typename comp_op>
	std::pair<typename Bst<key_type, value_type, comp_op>::iterator, bool> Bst<key_type, value_type, comp_op>::insert(pair_type&& x){
		if(!root){
			root = std::make_unique<node_type>(std::move(x));
			return std::make_pair<iterator,bool>(iterator(root.get()), true);
		}
		node_type* new_node = new node_type(std::move(x));
		node_type* tmp = root.get();
		while (tmp)
		{
			if(compare(x.first,tmp->value.first)){
				if(!tmp->left){
					new_node->parent = tmp;
					tmp->left.reset(new_node);
					return std::make_pair<iterator,bool>(iterator(new_node),true);
				}
				tmp = tmp->left.get();
			}
			else if (compare(tmp->value.first, x.first)){
				if(!tmp->right){
					new_node->parent = tmp;					
					tmp->right.reset(new_node);
					return std::make_pair<iterator,bool>(iterator(new_node),true);
				}
				tmp = tmp->right.get();
			}
			else 
				return std::make_pair<iterator,bool>(iterator(tmp), false);
		}
		
}

template <typename key_type, typename value_type, typename comp_op>
	typename Bst<key_type, value_type, comp_op>::iterator Bst<key_type, value_type, comp_op>::find(const key_type& x){
		node_type* tmp = root.get();
		while(tmp){
			if(compare(x,tmp->value.first)){
				tmp = tmp->left.get();
			}else if(compare(tmp->value.first, x)){
				tmp = tmp->right.get();
			}else
			{
				return iterator(tmp);
			}
			
		}
		return end();
} 

template <typename key_type, typename value_type, typename comp_op>
	size_t Bst<key_type, value_type, comp_op>::height(node_type* x){
		if(x){
			return 1+std::max(height(x->left.get()), height(x->right.get()));
		}
		return 0;
}

template <typename key_type, typename value_type, typename comp_op>
	bool Bst<key_type, value_type, comp_op>::isBalanced(node_type* x){
		if(!x){
			return 1;
		}
		size_t left_height, right_height;
		left_height = height(x->left.get());
		right_height = height(x->right.get());
		if(abs(left_height - right_height)<=1 && isBalanced(x->left.get()) && isBalanced(x->right.get()))
			return 1;
		
		return 0;
}

template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::balance_aux(std::vector<pair_type> v_balance, size_t size){
		if(size == 1){
			insert(v_balance.at(0));
		}else if (size == 2){
			insert(v_balance.at(1));
			insert(v_balance.at(0));
		}else {
			std::vector<pair_type> v_balance1(v_balance.begin(), v_balance.begin()+size/2);
			std::vector<pair_type> v_balance2(v_balance.begin()+size/2+1, v_balance.end());
			size = size/2;
			insert(v_balance.at(size));
			balance_aux(v_balance1, v_balance1.size());
			balance_aux(v_balance2, v_balance2.size());
		}
}

template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::balance(){

		if(isBalanced(root.get()))
			return;

		std::vector<pair_type> v_balance;
		auto it = cbegin();
		while(it != cend()){
			v_balance.push_back(*it);
			++it;
		}
		clear();
		balance_aux(v_balance, v_balance.size());
}

template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::swap_node(node_type* x, node_type* y){
	int chSide_x = childhoodSide(x);
	int chSide_y = childhoodSide(y);

	release_child(y->parent, chSide_y);
            
	if(y->right && y->parent != x){
        node_type* y_rightChild = y->right.release();
        reset_child(y->parent, y_rightChild, chSide_y);
        y_rightChild->parent = y->parent;
    }
                 
    y->left.release();
    if(x->left){
        node_type* x_leftChild = x->left.release();
        y->left.reset(x_leftChild);
		x_leftChild->parent = y;
    }
    if(x->right && x->right.get() != y){
        node_type* x_rightChild = x->right.release();
        y->right.reset(x_rightChild);
		x_rightChild->parent = y;
    }
                 
	if(x->parent){
		release_child(x->parent, chSide_x);
		reset_child(x->parent, y, chSide_x);
        y->parent = x->parent;
	}else{
		root.release();
		root.reset(y);
	}
}

template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::erase(const key_type& x){
		auto it = find(x);
		node_type* a = it.getCurrent();
		if(!a->left && !a->right){
			int chSide = childhoodSide(a);
			release_child(a->parent,chSide);
			delete a;
			return;
		}
		int chSide_a = childhoodSide(a);
		if(!a->left){
			node_type* a_right = a->right.release();
			release_child(a->parent, chSide_a);
			reset_child(a->parent, a_right, chSide_a);
			a_right->parent = a->parent;
			delete_node(a);
			return;
		}
		if(!a->right){
			node_type* a_left = a->left.release();
			release_child(a->parent, chSide_a);
			reset_child(a->parent, a_left, chSide_a);
			a_left->parent = a->parent;
			delete_node(a);
			return;
		}
		++it;
        node_type* b = it.getCurrent();
        swap_node(a,b);
		delete_node(a);
}

template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::bfs_aux(node_type* x, size_t level){
		if (!x)
			return;
		if(level == 1){
			pair_type val = x->value;
			std::cout << val.second << " ";
		}
		else{
			bfs_aux(x->left.get(), level-1 );
			bfs_aux(x->right.get(), level-1 );
		}
	}

template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::bfs(){
		size_t h = height(root.get());
		for(size_t i = 1; i <= h; i++)
			bfs_aux(root.get(), i);
		std::cout << std::endl;
	}