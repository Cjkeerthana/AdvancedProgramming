#ifndef __bst_hpp
#define __bst_hpp

#include <iostream>
#include <utility>
#include <memory>
#include <algorithm>
#include <vector>
#include <cmath>

#include "iterator.hpp"
/*
*************** Class BINARY SEARCH TREE ****************
*The binary search tree is implemented here where each node of the tree
*has a key of template key_type, a value of template value_type,
*and a comparison operator of template comp_op.
*The BST operates according to its operator and preserves it property
*i.e nodes with keys greater(as per the comparison operator) than a 
*particular node are on the right and the nodes with keys less than 
*it are on the left. 
*Each node has a parent and may or may not have a left or a right child
*It has a special node without a parent called the root.
*It supports the following operations
*1. INSERT --> Insert a value
*2. EMPLACE --> Insert value with the given args
*3. BALANCE --> Balance the tree
*4. ERASE  --> Erase a key from the tree
*/
template <typename key_type, typename value_type, typename comp_op = std::less<key_type>>
	class Bst{
        //Tempalted struct node
		template <typename T>
			struct node{
				T value;
				node* parent;	
				std::unique_ptr<node> left;		//unique pointer to the left child
				std::unique_ptr<node> right;	//unique pointer to the right child
							
				public:
					node(const T& v): value{v}, parent{nullptr} {}
					node(T &&v): value{std::move(v)}, parent{nullptr} {}

					node(const T& v, node* p): value{v}, parent{p} {}
					node(T &&v, node* p): value{std::move(v)}, parent{p} {}

					explicit node(const std::unique_ptr<node> &x, node* p): value{x->value} {
						this->parent = p;
						if(x->left)
							left = std::make_unique<node>(x->left,this);
						if(x->right)
							right = std::make_unique<node>(x->right,this);
					}
			};
            comp_op compare;

			using pair_type = std::pair<const key_type, value_type>;
			using node_type = node<pair_type>;

			std::unique_ptr<node_type> root;

            //some auxillary private functions 
            //To find the height of the tree/subtree starting with any node x
			size_t height(node_type* x) noexcept;

			//To check if the tree/subtree is balanced or not
			bool isBalanced(node_type* x) noexcept;
            //To balance the tree
			void balance_aux(std::vector<pair_type> v_balance, size_t size);
            //To tell what child is the node x; 1 if x is a right child, 0 if x is a right child, -1 if x doesnt have a parent (i.e. x is root)
            int childhoodSide(node_type* x) noexcept{
                if(x->parent){
				if(x->parent->left.get() == x) return 0;
				if(x->parent->right.get() == x) return 1;
                }
                return -1;
			}
            //To release a child of node x from the provided child side chSide 
            void release_child(node_type* x, int chSide) noexcept{
				 if(chSide == 1) x->right.release();
				 if(chSide == 0) x->left.release();
			}
            //To reset a child of given side of node x with the node y  
			void reset_child(node_type* x, node_type* y, int chSide) noexcept {
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
                Bst(key_type k, value_type v): compare{comp_op()}, root{std::make_unique<node_type>(std::pair<const key_type,value_type>(k,v))} {}
                Bst(key_type k, value_type v, comp_op comp): compare{comp}, root{std::make_unique<node_type>(std::pair<const key_type,value_type>(k,v))} {}

                //copy constructs
                Bst(const Bst& tree): compare{tree.compare} { root = std::make_unique<node_type>(tree.root,nullptr); }
                Bst& operator=(const Bst& tree){
                    if(&tree == this)
                        return *this;
                    this->clear();
                    compare = tree.compare;
                    root = std::make_unique<node_type>(tree.root,nullptr);
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
				const_iterator find(const key_type& x) const;

                //insert a value  ==> tree.insert({key,value})
                std::pair<iterator, bool> insert(const pair_type& x);
                std::pair<iterator, bool> insert(pair_type&& x);

				//Insert values both as pair_type or key_type,value_type
                template<class... Types>
			    std::pair<iterator,bool> emplace(Types&&... args) {return insert(pair_type(std::forward<Types>(args)...));}; 

				//To check if the tree is balanced or not
				bool check_balance() noexcept { return isBalanced(root.get()); }
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
                        auto t = insert({x,value_type{}});			
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
//BST OPERATIONS

/*
*******1. INSERT*******
* The function is used to insert a pair of key and value to the tree. 
*Used as follows:
*tree.insert({key,value});
*It takes as input a pair_type (std::pair<const key, value>) and return a pair containing 
*an iterator pointing to the inserted key(if inserted otherwise shows a nullptr) 
*and boolean value telling whether the key is sucessfully inserted or not
*/
template <typename key_type, typename value_type, typename comp_op>
	std::pair<typename Bst<key_type, value_type, comp_op>::iterator, bool> Bst<key_type, value_type, comp_op>::insert(const pair_type& x){
		node_type* tmp = root.get();
		if(!tmp){ // if the tree is empty, make the inserted pair as the root
			root = std::make_unique<node_type>(x);
			return std::make_pair<iterator,bool>(iterator(root.get()), true);
		}
		node_type* new_node = new node_type(x); //if not, form a new node
		while (tmp) //until we reach the end of the tree
		{
			if(compare(x.first,tmp->value.first)){ // Compare the new node with the root (comapring the key values)
				if(!tmp->left){					   // according to the comparison operator of the tree
					new_node->parent = tmp; 	   // Move to right child if it is greater(for std::less comparison)
					tmp->left.reset(new_node);	   // else move to the left child and repeat the same until reaching 
					return std::make_pair<iterator,bool>(iterator(new_node), true); // the end of the tree (i.e. nullptr)
				}
				tmp = tmp->left.get();				
			}
			else if (compare(tmp->value.first, x.first)){
				if(!tmp->right){
					new_node->parent = tmp;			// Set the parent for the new node and set the new node as the child of 
					tmp->right.reset(new_node);		//parent node.
					return std::make_pair<iterator,bool>(iterator(new_node), true);
				}
				tmp = tmp->right.get();
			}
			else 
				return std::make_pair<iterator,bool>(iterator(tmp), false);
		}
		return std::make_pair<iterator,bool>(iterator(tmp), false);
}

// The following function is the same insert operation when the key and value are to be moved

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
		return std::make_pair<iterator,bool>(iterator(tmp), false);
}
/*
********* 2. FIND **********
* The find function is used to locate a key in the tree
It takes as input a key and returns an iterator to the found key.
If the key is not found, it returns a nullptr
*/
template <typename key_type, typename value_type, typename comp_op>
	typename Bst<key_type, value_type, comp_op>::iterator Bst<key_type, value_type, comp_op>::find(const key_type& x){
		node_type* tmp = root.get();
		while(tmp){										//Start from the root
			if(compare(x,tmp->value.first)){			//Compare the the key with the root key
				tmp = tmp->left.get();					//Move left if the key < root key (for std::less)
			}else if(compare(tmp->value.first, x)){		//Move right if key > root
				tmp = tmp->right.get();					// Repeat it iteratively until we reach the end
			}else										// or find the key
			{
				return iterator(tmp);					
			}
			
		}
		return end(); // If key not found return the nullptr
} 

// The following function works the same way but returns a constant iterator

template <typename key_type, typename value_type, typename comp_op>
	typename Bst<key_type, value_type, comp_op>::const_iterator Bst<key_type, value_type, comp_op>::find(const key_type& x) const{
		node_type* tmp = root.get();
		while(tmp){
			if(compare(x,tmp->value.first)){
				tmp = tmp->left.get();
			}else if(compare(tmp->value.first, x)){
				tmp = tmp->right.get();
			}else
			{
				return const_iterator(tmp);
			}
			
		}
		return cend();
} 

/*
********** 3. BALANCE ***********
* Used to balance the tree.
* Needs some auxillary functions ==> a.height; b. isBalanced; c.balance_aux
* Works as tree.balance()
*/

//The auxillary functions
// ** a. height ** Used to calculate the height of the tree from a given node 
template <typename key_type, typename value_type, typename comp_op>
	size_t Bst<key_type, value_type, comp_op>::height(node_type* x) noexcept{
		if(x){
			return 1+std::max(height(x->left.get()), height(x->right.get()));
		}
		return 0;
}

// ** b. isBalanced ** To check if the tree is balanced or not. 
//Balanced tree ==> the difference in heightbetween the right and left subtrees do not exceed by 1
template <typename key_type, typename value_type, typename comp_op>
	bool Bst<key_type, value_type, comp_op>::isBalanced(node_type* x) noexcept{
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

//** c. balance_aux ** mainly does the balance operation.
//Takes as input a vector of pair_type values and builds the balanced tree
template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::balance_aux(std::vector<pair_type> v_balance, size_t size){
		if(size == 1){
			insert(v_balance.at(0));				//if the vector contains only one or two nodes insert them
		}else if (size == 2){
			insert(v_balance.at(1));
			insert(v_balance.at(0));
		}else {
			std::vector<pair_type> v_balance1(v_balance.begin(), v_balance.begin()+size/2); 
			std::vector<pair_type> v_balance2(v_balance.begin()+size/2+1, v_balance.end());	
			size = size/2;
			insert(v_balance.at(size)); //For more than two nodes, pick the middle element of the vector and insert it
			balance_aux(v_balance1, v_balance1.size()); //Copy the the other nodes into two vectors 
			balance_aux(v_balance2, v_balance2.size()); //and do the insertion recursively.
		}
}

/*
****** BALANCE ******
Used as tree.balance()
*/
template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::balance(){

		if(isBalanced(root.get()))		//if tree is already balanced do nothing
			return;

		std::vector<pair_type> v_balance; 	
		auto it = cbegin();
		while(it != cend()){			//else form a vector of the pair of keys and 
			v_balance.push_back(*it);	//values by doing an inorder walk of the tree
			++it;
		}
		clear();
		balance_aux(v_balance, v_balance.size());	//Give the vector to the auxillary function
}

/*
******* 4. ERASE *******
*Used to erase a key from the tree
* Auxiallary functions used a. swap_node
*/

//The following function is used to replace one node with another of the tree
//It transplants one node with another node, the parent and the children
//of the first node are set to the second node. 
//It is typically used when a node with two children has to erased.
template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::swap_node(node_type* x, node_type* y){
	int chSide_x = childhoodSide(x);	//determine the childhood side of the nodes x and y
	int chSide_y = childhoodSide(y);

	release_child(y->parent, chSide_y);	//Relase the node y from its parent node
            
	if(y->right && y->parent != x){		
        node_type* y_rightChild = y->right.release();	//If y has a right child and y is not a child of x
        reset_child(y->parent, y_rightChild, chSide_y); //Set the right child of y as the child of y's parent 
        y_rightChild->parent = y->parent;				//on the childhood side of y.
    }													// Make y's parent as the parent of y's right child.
                 
    y->left.release();									//By logic, y should not have a left child.
    if(x->left){										
        node_type* x_leftChild = x->left.release();	    //For the left child of x, set it as the left child of y
        y->left.reset(x_leftChild);						
		x_leftChild->parent = y;						//Set y as the parent of x's right child
    }
    if(x->right && x->right.get() != y){				//For the right child of x do the same only if y is not 
        node_type* x_rightChild = x->right.release();	//the right child of x
        y->right.reset(x_rightChild);
		x_rightChild->parent = y;
    }
                 
	if(x->parent){
		release_child(x->parent, chSide_x);				
		reset_child(x->parent, y, chSide_x);
        y->parent = x->parent;							//If x is not the root, set y's parent as x's parent 
	}else{
		root.release();									//If x is the root, reset the root to y
		root.reset(y);									// and set y's parent to nullptr
		y->parent = nullptr;
	}
}

/*
******** ERASE *********
* The erase function takes in a key as its argument and deletes the node 
* containing the key fromt he tree.
* used as tree.erase(key)
*/
template <typename key_type, typename value_type, typename comp_op>
	void Bst<key_type, value_type, comp_op>::erase(const key_type& x){
		auto it = find(x);								//Find the key
		if (it != end()){								
			node_type* a = it.getCurrent();
			if(!a->left && !a->right){					//If the key is found, check for the children
				int chSide = childhoodSide(a);			//of the corresponding node.
				release_child(a->parent,chSide);		//IF the node is a leaf, release it from
				delete a;								//its parent and delete the node.
				return;
			}
			int chSide_a = childhoodSide(a);			//If the node is not a leaf, see its childhood side
			if(!a->left){
				node_type* a_right = a->right.release();	//If the node lacks a left child, replace the node
				if(a == root.get()){						// with its left child
					root.release();							//If its the root & it lacks the left child
					root.reset(a_right);					//reset the root with the right child
					a_right->parent = nullptr;				// Set the right child parent to nullptr.
					delete_node(a);							
					return;
				}
				release_child(a->parent, chSide_a);			
				reset_child(a->parent, a_right, chSide_a);	
				a_right->parent = a->parent;				
				delete_node(a);
				return;
			}
			if(!a->right){
				node_type* a_left = a->left.release();	// If node lacks a right child, do similarly as when it 
				if(a == root.get()){					// a left child; replace the node with its left child
					root.release();
					root.reset(a_left);
					a_left->parent = nullptr;
					delete_node(a);
					return;
				}
				release_child(a->parent, chSide_a);
				reset_child(a->parent, a_left, chSide_a);
				a_left->parent = a->parent;
				delete_node(a);
				return;
			}
			++it;									//If the node has both the children, 
			node_type* b = it.getCurrent();			//go to the successor of the node
			swap_node(a,b);							//replace the node with its successor
			delete_node(a);							// Don't forget to delete the node everytime once the job is done ;)
		}
		else{										//If we try to erase a key which is not in the tree
			std::cout << "The given key doesn't exist" << std::endl;
		}
}

// A simple breadth first traversal of the tree
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

#endif