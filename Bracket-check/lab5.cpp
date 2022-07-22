#define SIZE 20
#include<string>

template<typename T>
class Stack {
	T dataArr_[SIZE];
	int top_;
public:
	Stack() {
		top_ = 0;
	}
	bool isEmpty() const {
		return top_ == 0;
	};
	bool isFull() const {
		return top_ == SIZE;
	}; 
	void push(T data) {
		if (!isFull()) {
			dataArr_[top_] = data;
			top_++;
		}
	};
	void pop() {
		if (top_ > 0) top_--;
	};
	T top() const {
		if (!isEmpty()) return dataArr_[top_ - 1];
	};
	
};

bool bracketCheck(const std::string& s){
	Stack<char> stack;

	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(' || s[i] == '{' || s[i] == '[') {
			stack.push(s[i]);
			continue;
		}

		if (s[i] == ')' || s[i] == '}' || s[i] == ']') {
			if (stack.isEmpty()) return false;

			char c = stack.top();
			stack.pop();
			if (c == '(') {
				if (s[i] != ')') return false;
			}
			else if (c == '{') {
				if (s[i] != '}') return false;
			}
			else {
				if (s[i] != ']') return false;
			}
		}
	}
	return stack.isEmpty();
}
