#include <iostream>

template <typename T>
class DList{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* next=nullptr, Node* prev=nullptr){
			data_=data;
			next_=next;
			prev_=prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	DList(){
		front_=nullptr;
		back_=nullptr;
	}
	void push_front(const T& data);
	~DList();

	class const_iterator{
		friend class DList;
		const DList* dlist_{ nullptr };
		Node* curr_;
		const_iterator(Node* n, const DList* d){
			curr_=n;
			dlist_=d;
		}
	public:
		const_iterator(){
			curr_=nullptr;
		}
		const_iterator& operator++(){
			curr_ = curr_->next_;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator old = *this;
			curr_ = curr_->next_;
			return old;
		};
		const_iterator& operator--() {
			if(curr_) curr_ = curr_->prev_;
			else curr_ = dlist_->back_;
			return *this;
		};
		const_iterator operator--(int) {
			const_iterator old = *this;
			if (curr_) curr_ = curr_->prev_;
			else curr_ = dlist_->back_;
			return old;
		};
		bool operator==(const_iterator rhs) { return curr_ == rhs.curr_; };
		bool operator!=(const_iterator rhs) { return curr_ != rhs.curr_; };
		const T& operator*()const { return curr_->data_; };
	};
	class iterator:public const_iterator{
		friend class DList;
		iterator(Node* curr, DList* d) : const_iterator(curr, d) {};
	public:
		iterator() : const_iterator() {};
		iterator& operator++() {
			this->curr_ = this->curr_->next_;
			return *this;
		};
		iterator operator++(int) {
			iterator old = *this;
			this->curr_ = this->curr_->next_;
			return old;
		};
		iterator& operator--() {
			if (this->curr_) this->curr_ = this->curr_->prev_;
			else this->curr_ = this->dlist_->back_;
			return *this;
		};
		iterator operator--(int) {
			iterator old = *this;
			if (this->curr_) this->curr_ = this->curr_->prev_;
			else this->curr_ = this->dlist_->back_;
			return old;
		};
		T& operator*() { return this->curr_->data_; };
		const T& operator*()const { return this->curr_->data_; };

	};

	const_iterator cbegin() const { return const_iterator(this->front_, this); }

	iterator begin() { return iterator(this-> front_, this); }

	const_iterator cend() const { return const_iterator(nullptr, this); }
	iterator end() { return iterator(nullptr, this); }
};

template <typename T>
void DList<T>::push_front(const T& data){
	Node* nn = new Node(data, front_, nullptr);
	if (front_) {
		front_->prev_ = nn;
	}
	else {
		back_ = nn;
	}
	front_ = nn;
}

template <typename T>
DList<T>::~DList(){
	while (back_) {
		Node* rm = back_;
		back_ = back_->prev_;
		if (back_) {
			back_->next_ = nullptr;
		}
		else {
			front_ = nullptr;
		}
		delete rm;
	}
}


template <typename T>
class Sentinel{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* next=nullptr, Node* prev=nullptr){
			data_=data;
			next_=next;
			prev_=prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	Sentinel(){
		front_=new Node();
		back_=new Node();
		front_->next_=back_;
		back_->prev_=front_;
	}
	void push_front(const T& data);
	~Sentinel();

	class const_iterator{
		//added friend
		friend class Sentinel;
		Node* curr_{ nullptr };
		const_iterator(Node* n) { curr_ = n; }
	public:
		const_iterator() {};
		const_iterator& operator++() {
			curr_ = curr_->next_;
			return *this;
		};
		const_iterator operator++(int) {
			const_iterator old = curr_;
			curr_ = curr_->next_;
			return old;
		};
		const_iterator& operator--() {
			curr_ = curr_->prev_;
			return *this;
		};
		const_iterator operator--(int) {
			const_iterator old = curr_;
			curr_ = curr_->prev_;
			return old;
		};
		bool operator==(const_iterator rhs) { return curr_ == rhs.curr_; };
		bool operator!=(const_iterator rhs) { return curr_ != rhs.curr_; };
		const T& operator*()const { return curr_->data_; };
	};
	class iterator:public const_iterator{
		//added friend
		friend class Sentinel;
		iterator(Node* n) : const_iterator(n) {}
	public:
		iterator(): const_iterator() {};
		iterator& operator++() {
			this->curr_ = this->curr_->next_;
			return *this;
		};
		iterator operator++(int) {
			iterator old = this->curr_;
			this->curr_ = this->curr_->next_;
			return old;
		};
		iterator& operator--() {
			this->curr_ = this->curr_->prev_;
			return *this;
		};
		iterator operator--(int) {
			iterator old = this->curr_;
			this->curr_ = this->curr_->prev_;
			return old;
		};
		T& operator*() { return this->curr_->data_; };
		const T& operator*()const { return this->curr_->data_; };
	};
	const_iterator cbegin() const { return const_iterator(this->front_->next_); };
	iterator begin() { return iterator(this->front_->next_); };
	const_iterator cend() const { return const_iterator(this->back_); };
	iterator end() { return iterator(this->back_); };

};

template <typename T>
void Sentinel<T>::push_front(const T& data){
	Node* nn = new Node(data, front_->next_, front_);
	front_->next_->prev_ = nn;
	front_->next_ = nn;
}


template <typename T>
Sentinel<T>::~Sentinel(){
	while (front_->next_ != back_) {
		Node* rm = back_;
		back_ = back_->prev_;
		if (back_) {
			back_->next_ = nullptr;
		}
		else {
			front_ = nullptr;
		}
		delete rm;
	}
	delete front_;
	delete back_;
}
