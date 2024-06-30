/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/

#include<algorithm>
#include <vector>
#include<iostream>
#include<fstream>

#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////

//Do not implement the class here, this is a forward declaration. Implement at
//the bottom of the .h file
template <class T> class BPlusTree; 

template <class T>
class BPlusTreeNode{
public:
	BPlusTreeNode() : parent(NULL) {};
	bool is_leaf();
	bool contains(const T& key);

	//For grading only. This is bad practice to have, because
	//it exposes a private member variable.
	BPlusTreeNode* get_parent() { return parent; } 

	//We need this to let BPlusTree access private members
	friend class BPlusTree<T>; 
private:
	bool contains(const T& key,std::size_t low,std::size_t high);
	std::vector<T> keys;
	std::vector<BPlusTreeNode*> children;
	BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
	for(unsigned int i=0; i<children.size(); i++){
		if(children[i]){
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
	if(low>high){
		return false;
	}
	if(low==high){
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]){
		return contains(key,0,mid);
	}
	else{
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////


template <class T>
class BPlusTree {
public:
	//constructors and destructor
	BPlusTree(int d) { degree = d; root = nullptr; }
	BPlusTree(const BPlusTree<T>& other);
	BPlusTree<T>& operator=(const BPlusTree<T>& other);
	~BPlusTree();

	//public member functions
	BPlusTreeNode<T>* find(const T& key) const;
	void insert(const T& key);
	void print_sideways(std::ostream& outfile) const;
	void print_BFS(std::ostream& outfile) const;

private:
	//private member functions
	void copy_children(BPlusTreeNode<T>* other, BPlusTreeNode<T>* current);
	void delete_node(BPlusTreeNode<T>* node);
	BPlusTreeNode<T>* find(const T& key, BPlusTreeNode<T>* node) const;
	void insert(const T& key, BPlusTreeNode<T>*& node);
	void split_node(BPlusTreeNode<T>* node);
	void print_sideways(std::ostream& outfile, BPlusTreeNode<T>* p, int depth) const;
	void print_BFS(std::ostream& outfile, std::vector<BPlusTreeNode<T>*>& nodes) const;
	void print_keys(std::ostream& outfile, BPlusTreeNode<T>* p) const;

	//member variables
	unsigned int degree;
	BPlusTreeNode<T>* root;

};

//copy constructor
template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) {
	degree = other.degree;
	if (other.root == nullptr) {
		root = nullptr;
	} else {
		root = new BPlusTreeNode<T>;
		root->keys = other.root->keys;
		if (!other.root->is_leaf()) {
			copy_children(other.root, root);
		}
	}
}

//helper function for copy constructor that copies the children of the old node to the new node
template <class T>
void BPlusTree<T>::copy_children(BPlusTreeNode<T>* other, BPlusTreeNode<T>* current) {
	for (unsigned int i = 0; i < other->children.size(); ++i) {
		BPlusTreeNode<T>* child;
		child = new BPlusTreeNode<T>;
		child->parent = current;
		child->keys = other->children[i]->keys;
		copy_children(other->children[i], child);
		current->children.push_back(child);
	}
}

//assignment operator
template <class T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& other) {
	if (other.root != nullptr) {
		//clear previous data in the tree
		if (root != nullptr) {
			for (unsigned int i = 0; i < root->children.size(); ++i) {
				delete_node(root->children[i]);
			}
			delete root;
			root = nullptr;
		}
		//copy data from other
		degree = other.degree;
		if (other.root != nullptr) {
			root = new BPlusTreeNode<T>;
			root->keys = other.root->keys;
			copy_children(other.root, root);
		}
	}

	return *this;
}

//destructor
template <class T>
BPlusTree<T>::~BPlusTree() {
	if (root != nullptr) {
		for (unsigned int i = 0; i < root->children.size(); ++i) {
			delete_node(root->children[i]);
		}
		delete root;
		root = nullptr;
	}
}

//helper function for destructor that deletes a specific node
template <class T>
void BPlusTree<T>::delete_node(BPlusTreeNode<T>* node) {
	if (!node->is_leaf()) {
		for (unsigned int i = 0; i < node->children.size(); ++i) {
			delete_node(node->children[i]);
		}
	}
	delete node;
	node = nullptr;
}

//public version of find available to user
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& key) const {
	if (root == nullptr) {
		return nullptr;
	}
	return find(key, root);
}

/*private, recursive version of find. If the key is not found in the tree, return a pointer to where
it should be.*/
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& key, BPlusTreeNode<T>* node) const {
	//if it's a leaf, the key belongs here
	if (node->is_leaf()) {
		return node;
	}
	//determine which child to search
	for (unsigned int i = 0; i < node->keys.size(); ++i) {
		if (key < node->keys[i]) {
			return find(key, node->children[i]);
		}
	}
	return find(key, node->children[node->children.size()-1]);
}

//public version of insert available to user
template <class T>
void BPlusTree<T>::insert(const T& key) {
	insert(key, root);
}

/*private, recursive version of insert that inserts the key into the correct location and adjusts
the tree so that it follows the rules of a B+ tree*/
template <class T>
void BPlusTree<T>::insert(const T& key, BPlusTreeNode<T>*& node) {
	//if empty tree
	if (node == nullptr) {
		node = new BPlusTreeNode<T>;
		node->keys.push_back(key);
		return;
	}

	//insert the key and handle splitting
	if (node->is_leaf()) {
		node->keys.push_back(key);
		std::sort(node->keys.begin(), node->keys.end());
		if (node->keys.size() >= degree) {
			split_node(node);
		}
		return;
	}

	//determine which child to insert the key in
	for (unsigned int i = 0; i < node->keys.size(); ++i) {
		if (key < node->keys[i]) {
			insert(key, node->children[i]);
			return;
		}
	}
	//insert into rightmost node if greater than all keys
	insert(key, node->children[node->children.size()-1]);

}

/*check if a node needs to be split, and if so split the node and recursively check if the parent 
now needs to be split*/
template <class T>
void BPlusTree<T>::split_node(BPlusTreeNode<T>* node) {
	BPlusTreeNode<T>* new_node;
	new_node = new BPlusTreeNode<T>;
	//move the second half of the keys to the new node
	unsigned int mid = node->keys.size()/2;
	for (unsigned int i = mid; i < node->keys.size(); ++i) {
		new_node->keys.push_back(node->keys[i]);
	}
	for (unsigned int i = 0; i < new_node->keys.size(); ++i) {
		node->keys.pop_back();
	}
	//move the second half of children to the new node and reassign parent pointers
	if (!node->is_leaf()) {
		mid = (node->children.size()+1)/2;
		for (unsigned int i = mid; i < node->children.size(); ++i) {
			new_node->children.push_back(node->children[i]);
			node->children[i]->parent = new_node;
		}
		for (unsigned int i = 0; i < new_node->children.size(); ++i) {
			node->children.pop_back();
		}
	}

	//move the appropriate key up to the parent node
	if (node->parent == nullptr) { //if this is the root node
		BPlusTreeNode<T>* new_root;
		new_root = new BPlusTreeNode<T>;
		node->parent = new_root;
		new_node->parent = new_root;
		new_root->keys.push_back(new_node->keys[0]);
		new_root->children.push_back(node);
		new_root->children.push_back(new_node);
		if (!new_node->is_leaf()) {
			new_node->keys.erase(new_node->keys.begin());
		}
		root = new_root;
	} else {
		new_node->parent = node->parent;
		node->parent->keys.push_back(new_node->keys[0]);
		std::sort(node->parent->keys.begin(), node->parent->keys.end());
		//find where to insert the new node
		typename std::vector<BPlusTreeNode<T>*>::iterator itr;
		itr = std::find(node->parent->children.begin(), node->parent->children.end(), node);
		++itr;
		node->parent->children.insert(itr, new_node);
		if (!new_node->is_leaf()) {
			new_node->keys.erase(new_node->keys.begin());
		}
		//check if parent now needs to be split
		if (node->parent->keys.size() >= degree) {
			split_node(node->parent);
		}
	}
}

//public version of print_sideways available to user
template <class T>
void BPlusTree<T>::print_sideways(std::ostream& outfile) const {
	if (root == nullptr) {
		outfile << "Tree is empty." << std::endl;
	} else {
		print_sideways(outfile, root, 0);
	}
}

//private, recursive version that prints a sideways version of the tree
template <class T>
void BPlusTree<T>::print_sideways(std::ostream& outfile, BPlusTreeNode<T>* p, int depth) const {
    if (p) {
    	unsigned int mid = degree/2;
    	//print first half of children
    	for (unsigned int i = 0; i < mid && i < p->children.size(); ++i) {
    		print_sideways(outfile, p->children[i], depth+1);
    	}
    	
    	//print this node's keys
    	for (int i=0; i<depth; ++i) {
      		outfile << "\t";
      	}
      	print_keys(outfile, p);
      	outfile << std::endl;
      	
      	//print second half of children
      	for (unsigned int i = mid; i < p->children.size(); ++i) {
      		print_sideways(outfile, p->children[i], depth+1);
      	}
    }
}

//public version of print_BFS available to the user
template <class T>
void BPlusTree<T>::print_BFS(std::ostream& outfile) const {
	if (root == nullptr) {
		outfile << "Tree is empty." << std::endl;
	} else {
		std::vector<BPlusTreeNode<T>*> nodes;
		nodes.push_back(root);
		print_BFS(outfile, nodes);
	}
}

//private, recursive version that prints the tree using a breadth-first method
template <class T>
void BPlusTree<T>::print_BFS(std::ostream& outfile, std::vector<BPlusTreeNode<T>*>& nodes) const {
	if (nodes.size() > 0) {
		//print the keys of each node in nodes
      	for (unsigned int i = 0; i < nodes.size(); ++i) {
      		if (i > 0) {
      			outfile << "\t";
      		}
      		print_keys(outfile, nodes[i]);
      	}
      	outfile << std::endl;

      	//replace the nodes vector with a vector of their children
      	std::vector<BPlusTreeNode<T>*> new_nodes;
      	for (unsigned int i = 0; i < nodes.size(); ++i) {
      		for (unsigned int j = 0; j < nodes[i]->children.size(); ++j) {
      			new_nodes.push_back(nodes[i]->children[j]);
      		}
      	}
      	nodes = new_nodes;
      	print_BFS(outfile, nodes);
	}
}


//helper function that prints the keys of the given node
template <class T>
void BPlusTree<T>::print_keys(std::ostream& outfile, BPlusTreeNode<T>* p) const {
	if (p && p->keys.size() > 0) {
		outfile << p->keys[0];
      	for (unsigned int i = 1; i < p->keys.size(); ++i) {
      		outfile << ',' << p->keys[i];
      	}
	}
}

#endif
