#ifndef __bst_hpp
#define __bst_hpp

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "iterator.hpp"


template <typename key_type, typename value_type, typename comp_op = std::less<key_type>>
	class Bst{
        //Tempalted struct node
		template <typename T>
			struct node{
				std::unique_ptr<node> left;
				std::unique_ptr<node> right;
				node* parent;				
				T value;

				public:
					node(const T& v): value{v}, parent{nullptr} {}
					node(T &&v): value{std::move(v)}, parent{nullptr} {}

					node(const T& v, node* p): value{v}, parent{p} {}
					node(T &&v, node* p): value{std::move(v)}, parent{p} {}

					explicit node(const std::unique_ptr<node> &x, node* p): value{x->value} {
						this->parent = p;
						if(x->left)
							this->left = std::make_unique<node>(x->left);
						if(x->right)
							this->right = std::make_unique<node>(x->right);
					}
			};
            comp_op compare;

			using pair_type = std::pair<const key_type, value_type>;
			using node_type = node<pair_type>;

			std::unique_ptr<node_type> root;

            //some auxillary private functions 
            //To find the height of the tree/subtree starting with any node x
			size_t height(node_type* x);
            //To check if the tree/subtree is balanced or not
			bool isBalanced(node_type* x);
            //To balance the tree
			void balance_aux(std::vector<pair_type> v_balance, size_t size);
            //To tell what child is the node x; 1 if x is a right child, 0 if x is a right child, -1 if x doesnt have a parent (i.e. x is root)
            int childhoodSide(node_type* x){
                if(x->parent){
				if(x->parent->left.get() == x) return 0;
				if(x->parent->right.get() == x) return 1;
                }
                return -1;
			}
            //To release a child of node x from the provided child side chSide 
            void release_child(node_type* x, int chSide){
				 if(chSide == 1) x->right.release();
				 if(chSide == 0) x->left.release();
			};
            //To reset a child of given side of node x with the node y  
			void reset_child(node_type* x, node_type* y, int chSide){
				if(chSide == 1) x->right.reset(y);
				if(chSide == 0) x->left.reset(y);
			}
            //To swap two nodes -- the children and the parent are swapped
            void swap_node(node_type* x, node_type* y);
            //The right and left children of node x are released and the node is deleted 
            void delete_node(node_type* x){
                if(x->right) x->right.release();
                if(x->left) x->left.release();
                delete x;
            }
            //To do a breadth first traversal in the tree
			void bfs_aux(node_type* x, size_t level);
            
            public:
                Bst(): compare{comp_op()}, root{nullptr} {}
                Bst(comp_op comp): compare{comp}, root{nullptr} {}
                Bst(key_type k, value_type v): compare{comp_op()}, root{std::make_unique<node_type>(std::make_pair<const key_type,value_type>(k,v))} {}
                Bst(key_type k, value_type v, comp_op comp): compare{comp}, root{std::make_unique<node_type>(std::make_pair<const key_type,value_type>(k,v))} {}

                //copy constructs
                Bst(const Bst& tree): comp_op{tree.compare} { root = std::make_unique<node_type>(tree.root); }
                Bst& operator=(const Bst& tree){
                    if(&tree == this)
                        return *this;
                    this->clear();
                    auto tmp = tree;
                    this->compare = tree.compare;
                    this->root = std::make_unique<node_type>(tree.root);
                    return *this;
                }

                //move constructs
                Bst(Bst&& tree) noexcept = default;
                Bst& operator=(Bst &&tree) noexcept = default;

                //iterator functions           
                using iterator = __iterator<node_type, pair_type>;
			    using const_iterator = __iterator<node_type, const pair_type>;

                iterator begin() noexcept { 
				node_type* x = root.get();
				while(x->left)
					x = x->left.get();
				return iterator(x); 
                }
                iterator end() noexcept { return iterator{nullptr}; }

                const_iterator begin() const { 
                    node_type* x = root.get();
                    while(x->left)
                        x = x->left.get();
                    return const_iterator(x); 
                }
                const_iterator end() const { return const_iterator{nullptr}; }

                const_iterator cbegin() const { 
                    node_type* x = root.get();
                    while(x->left)
                        x = x->left.get();
                    return const_iterator(x); 
                }
                const_iterator cend() const { return const_iterator{nullptr}; }


                //Operations on the tree
                
                //find a value 
                iterator find(const key_type& x);

                //insert a value  ==> tree.insert({key,value})
                std::pair<iterator, bool> insert(const pair_type& x);
                std::pair<iterator, bool> insert(pair_type&& x);

                template<class... Types>
			    std::pair<iterator,bool> emplace(Types&&... args) {return insert(pair_type(std::forward<Types>(args)...));}; 

                //Balance the tree ==> tree.balance();
                void balance();                
                //Clear the entire tree ==> tree.clear();
                void clear() noexcept { if(root) root.reset(); }
                //Erase the node associated with the particular key x ==> tree.erase(key)
                void erase(const key_type& x);
                //Perform a bfs traversal on the tree and print the nodes ==> tree.bfs
                void bfs();

                //Operator overloading
                value_type& operator[](const key_type& x){
                    auto it = find(x);
                    if(it == end()){
                        auto t = insert({x,value_type{}});			//the key is inserted if the key is accessed using [] operator is this fine?
                        it = t.first;
                    }
                    return (*it).second;
                }
                value_type& operator[](key_type&& x){
                    auto it = find(x);
                    if(it == end()){
                        auto t = insert({std::move(x),value_type{}});
                        it = t.first;
                    }
                    return (*it).second;
                }
                //on printing the tree, the tree follows inorder traversal.
                friend std::ostream& operator<<(std::ostream& os, const Bst& tree){
                    auto it = tree.cbegin();
                    while(it != tree.cend()){
                        os << (*it).second << " ";
                        ++it;
                    }
                    return os;
                }

};

#endif