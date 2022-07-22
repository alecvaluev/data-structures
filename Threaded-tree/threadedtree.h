#include <iostream>
using namespace std;

template <class T>
class ThreadedTree
{	
	struct Node{
		T data_;
		Node* left_;
		Node* right_;
		bool leftThread_;
		bool rightThread_;
		Node(const T& data = T{}, Node* left = nullptr, Node* right = nullptr){
			data_ = data;
			right_ = right;
			left_ = left;
			leftThread_ = true;
			rightThread_ = true;
		}

	};

	Node* root_;
	Node* smallest_; // points to the node containing the smallest value
	Node* biggest_;   // points to the node containing the largest value
	int size_;

public:

	class const_iterator
	{
	protected:
		Node* curr_;
		const ThreadedTree* tree_;

		const_iterator(Node* n, const ThreadedTree* t){
			curr_ = n;
			tree_ = t;
		}

	public:
		const_iterator(){
			curr_ = nullptr;
			tree_ = nullptr;
		}

		//The decrement postfix constant operator calculates the constant iterator pointing to the node with the value smaller than the current node.
		//if the current operator is null then the operator will assign the current node to the to the node with the biggest value and will return the previous constant iterator with the previous value
		const_iterator operator--(int){
			const_iterator old = *this;  // return this iterator after making the nessescary changes
			if (this->curr_){

				if (this->curr_->leftThread_) {
					this->curr_ = this->curr_->left_;
				}
				else {
					this->curr_ = this->curr_->left_;
					while (!this->curr_->rightThread_) { 
						this->curr_ = this->curr_->right_;
					}
				}
			}
			else {
				this->curr_ = this->tree_->biggest_;
			}
			return old;
		}

		// increment prefix operator returns iterator pointing to the node with the next bigger value. 
		// If the iterator given is pointing to null pointer, then, operator will cast and return iterator pointing to null pointer. 
		const_iterator operator++(){
			if (this->curr_){

				if (this->curr_->rightThread_) {
					this->curr_ = this->curr_->right_;
				}
				else {
					this->curr_ = this->curr_->right_;
					while (!this->curr_->leftThread_) {
						this->curr_ = this->curr_->left_;
					}
				}
			}
			return *this;
		}

		// increment postfix operator calculates the iterator pointing to the node with the next bigger value and returns the iterator with it's previous state
		// If the iterator given is pointing to null pointer, then, operator will instead  return iterator pointing to null pointer. 
		const_iterator operator++(int) {
			const_iterator old = *this;		// return this iterator after making the nessescary changes
			if (this->curr_)
			{

				if (this->curr_->rightThread_) {
					this->curr_ = this->curr_->right_;
				}
				else {
					this->curr_ = this->curr_->right_;
					// Finding the left most node
					while (!this->curr_->leftThread_) {
						this->curr_ = this->curr_->left_;
					}
				}


			}

			return old;


		}
		//The decrement prefix constant operator calculates the constant iterator pointing to the node with the value smaller than the current node.
		//if the current operator is null then the operator will return the constant constant iterator pointing to the node with the biggest value
		const_iterator operator--(){
			if (this->curr_){

				if (this->curr_->leftThread_) {
					this->curr_ = this->curr_->left_;
				}
				else {
					this->curr_ = this->curr_->left_;
					while (!this->curr_->rightThread_) {
						this->curr_ = this->curr_->right_;
					}
				}
			}
			else
				this->curr_ = this->tree_->biggest_;

			return *this;
		}
		//This function returns the current unmodifiable data held by the current iterator.
		const T& operator*() const{
			return this->curr_->data_;
		}
		//The comparison operator takes a constant iterator and checks if the current pointer is the same as the one of the parameter passed
		//in case both are the same the function returns true otherwise false
		bool operator==(const const_iterator& rhs) const{
			return this->curr_ == rhs.curr_;
		}
		//The comparison operator takes a constant iterator and checks if the current pointer is not the same as the one of the parameter passed
		//in case both are the same the function returns false otherwise true
		bool operator!=(const const_iterator& rhs) const{
			return this->curr_ != rhs.curr_;
		}
		friend class ThreadedTree;
	};
	class iterator :public const_iterator
	{
		iterator(Node* curr, ThreadedTree* t) : const_iterator(curr, t) {};
	public:
		iterator() :const_iterator() {}
		
		//This function returns the current data held by the current iterator.
		T& operator*(){ return this->curr_->data_; }

		// increment prefix operator returns iterator pointing to the node with the next bigger value. 
		// If the iterator given is pointing to null pointer, then, operator will return the iterator pointing to null pointer
		iterator operator++(){
			if (this->curr_)
			{

				if (this->curr_->rightThread_) {
					this->curr_ = this->curr_->right_;
				}
				else {
					this->curr_ = this->curr_->right_;
					// Finding the left most node
					while (!this->curr_->leftThread_) {
						this->curr_ = this->curr_->left_;
					}
				}


			}

			return *this;
		}
		//The decrement postfix operator calculates the iterator pointing to the node with the value smaller than the current node.
		//if the current operator is null then the operator will assign the current node to the to the node with the biggest value and will return 
		// the iterator with the previous value
		iterator operator--(int){
			iterator old = *this;
			if (this->curr_){
				if (this->curr_->leftThread_) 
					this->curr_ = this->curr_->left_;
				else {
					this->curr_ = this->curr_->left_;
					// Finding the left most node
					while (!this->curr_->rightThread_) {
						this->curr_ = this->curr_->right_;
					}
				}


			}
			else {
				this->curr_ = this->tree_->biggest_;
			}
			return old;
		}

		// increment postfix operator calculates the iterator pointing to the node with the next bigger value and returns the iterator with it's previous state
		// If the iterator given is pointing to null pointer, then, operator will instead return the current iterator pointing to null pointer
		iterator operator++(int) {

			iterator old = *this; // assigning the previous state of the current iterator
			if (this->curr_){

				if (this->curr_->rightThread_)
					this->curr_ = this->curr_->right_;
				else {
					this->curr_ = this->curr_->right_;
					while (!this->curr_->leftThread_) {
						this->curr_ = this->curr_->left_;
					}
				}


			}

			return old;
		}
		//The decrement prefix operator calculates the iterator pointing to the node with the value smaller than the current node.
		//if the current operator is null then the operator will return the iterator pointing to the node with the biggest value
		iterator operator--(){
			if (this->curr_){

				if (this->curr_->leftThread_) {
					this->curr_ = this->curr_->left_;
				}
				else {
					this->curr_ = this->curr_->left_;
					while (!this->curr_->rightThread_) {
						this->curr_ = this->curr_->right_;
					}
				}


			}
			else {
				this->curr_ = this->tree_->biggest_;
			}
			return *this;
		}

		friend class ThreadedTree;
	};

	ThreadedTree(){
		root_ = nullptr;
		biggest_ = nullptr;
		smallest_ = nullptr;
		size_ = 0;
	}
	
	/*The insert function takes data as its only parameter, creates a new node containing that data, and then calculates 
	*where the new node must be inserted by comparing values in the binary tree.
	*The function also checks if the newest value would be the smallest or the biggest in the tree and if it is then it 
	*assign a pointer to it.
	*/
	void insert(const T& data){
		Node* leftThread = nullptr;
		Node* rightThread = nullptr;

		if (!root_) {
			root_ = new Node(data);
			this->biggest_ = root_;
			this->smallest_ = root_;
		}
		else {
			Node* curr = root_;
			while (curr) {
				if (data < curr->data_) {
					rightThread = curr;
					//go left
					if (curr->leftThread_) {
						//no node
						curr->left_ = new Node(data, leftThread, rightThread);
						curr->leftThread_ = false;

						if (data < this->smallest_->data_) 
							this->smallest_ = curr->left_;
						
						curr = nullptr;
					}
					else {
						curr = curr->left_;
					}
				}
				else {
					leftThread = curr;
					//go right
					if (curr->rightThread_) {
						curr->right_ = new Node(data, leftThread, rightThread);
						curr->rightThread_ = false;

						if (this->biggest_->data_ < data) {
							this->biggest_ = curr->right_;
						}
						curr = nullptr;
					}
					else {
						curr = curr->right_;
					}
				}
			}
		}
		size_++;
	}

	/*The find function takes key as its only parameter, initializes a iterator, traverses through the tree and compares the key
	  If key is not found, iterator pointing to the end of the tree is returned. Otherwise, the iterator pointing to the same key is returned
	*/
	iterator find(const T& key){
		iterator iter = end();
		for (auto it = begin(); it != end(); it++)
		{
			if ((*it) == key)
			{
				iter = it;
				break;
			}
		}
		return iter;
	}

	/*The find function takes key as its only parameter, initializes a constant iterator, traverses through the tree and compares the key
	  If key is not found, constant iterator pointing to the end of the tree is returned. Otherwise, the constant iterator pointing to the same key is returned
	*/
	const_iterator find(const T& key) const{
		const_iterator iter = cend();
		for (auto it = cbegin(); it != cend(); it++)
		{
			if ((*it) == key)
			{
				iter = it;
				break;
			}
		}
		return iter;
	}


	// This function returns an iterator with the smallest node in the tree. 
	iterator begin(){
		return iterator(this->smallest_, this);
	}

	// This function returns a unmodifiable iterator with the smallest node in the tree. 
	const_iterator cbegin() const{
		return const_iterator(this->smallest_, this);
	}

	// this function return a iterator one after the end. 
	iterator end(){
		return iterator(nullptr, this);
	}

	//this function returns a unmodifiable iterator one after the end .
	const_iterator cend() const{
		return const_iterator(nullptr, this);
	}


	void print() const {}

	//this function return the total number of nodes in the tree.
	int size() const{
		return size_;
	}
	//this function returns true if the tree is empty by comparing if size is equal to 0.
	bool empty() const{
		return (size() == 0);
	}

	~ThreadedTree(){
		destroyTree(root_);
	}

	//this function destroys given node given that it is a leaf. Otherwise, recursively calls function itself by passing down the left
	// and or the right children of the givenNode. 
	void destroyTree(Node* givenNode){
		if (givenNode)
		{
			if (!givenNode->leftThread_)
				destroyTree(givenNode->left_);
			if (!givenNode->rightThread_)
				destroyTree(givenNode->right_);

			delete givenNode;
			givenNode = nullptr;
		}
	}

};
