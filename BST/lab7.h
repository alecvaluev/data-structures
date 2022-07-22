#include <iostream>
#include <iomanip>

/*forward declaration*/
template <class T>
class Queue;

template <class T>
class BST{
	struct Node{
		T data_;
		Node* left_;
		Node* right_;
		Node(const T& data,Node* lt=nullptr,Node* rt=nullptr){
			data_=data;
			left_=lt;
			right_=rt;
		}
	};
	Node* root_;

	void printPreOrder(Node* subroot)const{
		if(subroot){
			std::cout << subroot->data_ << " ";		
			printPreOrder(subroot->left_);
			printPreOrder(subroot->right_);
		}
	}
	void printInOrder(Node* subroot)const{
		if(subroot){
			printInOrder(subroot->left_);
			std::cout << subroot->data_ << " ";
			printInOrder(subroot->right_);
		}
	}
	void destroy(Node* subroot){
		if(subroot){
			destroy(subroot->left_);
			destroy(subroot->right_);
			delete subroot;
		}
	}
	
	bool isSame(const Node* left, const Node* right) const;

	/*used by print() to print all nodes at same level*/
	void printLine(Node* data[],int numNodes,int width) const{
		int half=width/2;
		int firsthalf=width%2?half+1:half;

		if(numNodes > 1){
			for(int i=0;i<numNodes;i++){
				if(i%2==0){
					if(data[i]){
						std::cout << std::right <<std::setfill(' ') << std::setw(firsthalf)<< "-";
						std::cout << std::left << std::setfill('-') << std::setw(half) << "-";
					}
					else{
						std::cout << std::right <<std::setfill(' ') << std::setw(firsthalf)<< " ";
						std::cout << std::left << std::setfill(' ') << std::setw(half) << " ";
					}
				}
				else{
					if(data[i]){
						std::cout << std::right << std::setfill('-') << std::setw(firsthalf) << "-";
						std::cout << std::left <<std::setfill(' ') << std::setw(half)<<"-";
					}
					else{
						std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
						std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
					}
				}
			}
			std::cout << std::endl;
		}
		for(int i=0;i<numNodes;i++){
			if(data[i]){
				if(i%2==0){
					std::cout << std::right << std::setw(firsthalf) << "|";
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
				}
				else{
					std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<< "|";				
				}
			}
			else{
				std::cout << std::left <<std::setfill(' ') << std::setw(width)<<" ";			
			}
		}
		std::cout << std::endl;		
		for(int i=0;i<numNodes;i++){
			if(data[i]){
				if(i%2==0){
					std::cout << std::right << std::setw(firsthalf) << data[i]->data_;
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<<" ";
				}
				else{
					std::cout << std::right << std::setfill(' ') << std::setw(firsthalf) << " ";
					std::cout << std::left <<std::setfill(' ') << std::setw(half)<< data[i]->data_;				
				}
			}
			else{
				std::cout << std::left <<std::setfill(' ') << std::setw(width)<<" ";			
			}
		}
		std::cout << std::endl;

	}


public:
	BST(){
		root_=nullptr;
	}

	BST(const BST& rhs){
		if(this != &rhs && rhs.root_)
		{
			root_ = cloneNode(rhs.root_);
		}
	}

	Node* cloneNode(Node* curr) const
	{
		if (curr)
		{
			Node* newNode = new Node(curr->data_, cloneNode(curr->left_), cloneNode(curr->right_));
			return newNode;
		}
		return nullptr;
	}
	void remove(const T& data){
		//tree has nodes
		if (root_)
		{
			Node* parent = nullptr;
			Node* curr = root_;

			//look for the node
			while (curr->data_ != data)
			{
				//if data smaller go left
				if (curr->data_ > data)
				{
					parent = curr;
					curr = curr->left_;
				}
				else
				{
					parent = curr;
					curr = curr->right_;
				}
			}

			//1.node has no children
			if (curr->left_ == nullptr && curr->right_ == nullptr)
			{
				if (parent)  //is there a parent
				{
					if (parent->left_ == curr)
					{
						parent->left_ = nullptr;
					}
					else
					{
						parent->right_ = nullptr;
					}
				}
				else
				{
					root_ = nullptr;
				}
			}
			//2.node has 2 children
			else if (curr->left_ && curr->right_)
			{
				Node* successorParent = nullptr;
				Node* successor = nullptr;
				
				if (curr->right_)
				{
					successorParent = curr;
					successor = curr->right_;

					//find node with the smallest value
					while (successor->left_)
					{
						//do i need to check the right value
						successorParent = successor;
						if (successor->left_) successor = successor->left_;
					}
				}
				
				//remove the successor
				if (successorParent->right_ == successor)
				{
					successorParent->right_ = successor->right_;
				}
				else
				{
					successorParent->left_ = successor->right_;
				}

				//replace curr node with the successor
				if (parent)
				{
					if (parent->left_ == curr)
					{
						parent->left_ = successor;

					}
					else
					{
						parent->right_ = successor;
					}
				}
				else
				{
					root_ = successor;
				}
				
				successor->left_ = curr->left_;
				successor->right_ = curr->right_;

			}
			//3.node has 1 child
			else {
				Node* child;
				if (curr->left_)
				{
					child = curr->left_;
				}
				else
				{
					child = curr->right_;
				}
				if (parent)
				{
					if (parent->left_ == curr) //on the left of the parent
					{
						parent->left_ = child;
					}
					else   //on the right of the parent
					{
						parent->right_= child;
					}
				}
				else
				{
					root_ = child;
				}
			}

			//delete the node
			delete[] curr;
		}
	}

	int depth(const T& data){
		int depthValue = 0;
		Node* curr = root_;
		
		while (curr->data_ != data)
		{
			//if data smaller go left
			if (curr->data_ > data)
			{
				if (curr->left_) curr = curr->left_;
				else
				{
					depthValue = -1;
					break;
				}
			}
			else
			{
				if(curr->right_) curr = curr->right_;
				else
				{
					depthValue = -1;
					break;
				}
			}
			depthValue++;
		}

		return depthValue;
	}


	void printPreOrder() const{
		printPreOrder(root_);
		std::cout << std::endl;
	}
	void printInOrder() const{
		printInOrder(root_);
		std::cout << std::endl;
	}


	void insert(const T& data){
		if(root_==nullptr){
			root_=new Node(data);
		}
		else{

			Node* curr=root_;
			while(curr != nullptr){
				if(data < curr->data_){
					//go left
					if(curr->left_){
						curr=curr->left_;
					}
					else{
						curr->left_=new Node(data);
						curr=nullptr;
					}
				}	
				else{
					//go right
					if(curr->right_){
						curr=curr->right_;
					}
					else{
						curr->right_=new Node(data);
						curr=nullptr;
					}
				}
			}

		}

	}

	bool operator==(const BST& rhs) const;
	void print() const{
		struct Output{
			Node* node_;
			int lvl_;
			int position_;
			Output(Node* n=nullptr,int l=0, int p=0){
				node_=n;
				lvl_=l;
				position_=p;
			}
			void set(Node* n=nullptr,int l=0, int p=0){
				node_=n;
				lvl_=l;
				position_=p;
			}
		};
		Queue<Output> theNodes;
		Node* line[16];
		if(root_){
			for(int i=0;i<16;i++){
				line[i]=nullptr;
			}
			theNodes.enqueue(Output(root_,0,0));
			int currline=0;
			int width=80;
			int numInLine=1;
			while(theNodes.isEmpty()==false){
				Output curr=theNodes.front();
				if(curr.node_->left_)
					theNodes.enqueue(Output(curr.node_->left_,curr.lvl_+1,curr.position_*2));
				if(curr.node_->right_)
					theNodes.enqueue(Output(curr.node_->right_,curr.lvl_+1,curr.position_*2+1));
				theNodes.dequeue();


				if(curr.lvl_>currline){
					printLine(line,numInLine,width);
					width=width/2;
					numInLine=numInLine*2;
					for(int i=0;i<16;i++){
						line[i]=nullptr;
					}
					currline++;

				}
				line[curr.position_]=curr.node_;
			}
			printLine(line,numInLine,width);
			std::cout << std::endl;

		}
		else{
			std::cout << "tree is empty" << std::endl;
		}
	}
	~BST(){
		destroy(root_);
	}
};

template <class T>
class Queue{
	T* theQueue_;
	int capacity_;
	int used_;
	int front_;
	int back_;
	void grow(){
		T* tmp=new T[capacity_*2];
		int j;
		for(int i=0,j=front_;i<used_;i++,j=(j+1)%capacity_){
			tmp[i]=theQueue_[j];
		}
		delete [] theQueue_;
		theQueue_=tmp;
		capacity_=capacity_*2;
		front_=0;
		back_=used_;
	}
public:
	Queue(){
		theQueue_=new T[50];
		capacity_=50;
		used_=0;
		front_=0;
		back_=0;
	}
	void enqueue(const T& data){
		if(used_==capacity_){
			grow();
		}
		theQueue_[back_]=data;
		back_=(back_+1)%capacity_;
		used_++;
	}
	void dequeue(){
		if(!isEmpty()){
			used_--;
			front_=(front_+1)%capacity_;
		}
	}
	T front() const{
		if(!isEmpty()){
			return theQueue_[front_];
		}
		return T{};

	}
	bool isEmpty() const{
		return used_==0;
	}
	~Queue(){
		delete [] theQueue_;
	}	
};
