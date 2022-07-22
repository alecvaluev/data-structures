/********************************************************************/
/*                                                                  */
/*  A1: DList starter file                                   */
/*                                                                  */
/*  Author1 Name: Aleksandr Valuev                                  */
/*      - class/function list/main author or main checker           */
/*                                                                  */
/********************************************************************/
#include <iostream>
#include <utility>

template <typename T>

class DList
{
	struct Node
	{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data = T{}, Node* next = nullptr, Node* prev = nullptr)
		{
			data_ = data;
			next_ = next;
			prev_ = prev;
		}
	};
	Node* front_;
	Node* back_;

public:
	class const_iterator
	{
		friend class DList;
		const DList* dlist_{ nullptr };
		Node* curr_;
		const_iterator(Node* n, const DList* d)
		{
			curr_ = n;
			dlist_ = d;
		}
	public:
		const_iterator()
		{
			curr_ = nullptr;
		}
		const_iterator& operator++()
		{
			if (curr_) curr_ = curr_->next_;
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator old = *this;
			if (curr_) curr_ = curr_->next_;
			return old;
		}
		const_iterator& operator--()
		{
			if (curr_) curr_ = curr_->prev_;
			else curr_ = dlist_->back_;
			return *this;
		}
		const_iterator operator--(int)
		{
			const_iterator old = *this;
			if (curr_) curr_ = curr_->prev_;
			else curr_ = dlist_->back_;
			return old;
		}
		bool operator==(const_iterator rhs)
		{
			return curr_ == rhs.curr_;
		}
		bool operator!=(const_iterator rhs)
		{
			return curr_ != rhs.curr_;
		}
		const T& operator*()const
		{
			return curr_->data_;
		}
	};
	class iterator :public const_iterator
	{
		friend class DList;
		iterator(Node* curr, DList* d) : const_iterator(curr, d)
		{
		};
	public:
		iterator() : const_iterator()
		{
		};
		iterator& operator++()
		{
			if (this->curr_) this->curr_ = this->curr_->next_;
			return *this;
		}
		iterator operator++(int)
		{
			iterator old = *this;
			if (this->curr_) this->curr_ = this->curr_->next_;
			return old;
		}
		iterator& operator--()
		{
			if (this->curr_) this->curr_ = this->curr_->prev_;
			else this->curr_ = this->dlist_->back_;
			return *this;
		}
		iterator operator--(int)
		{
			iterator old = *this;
			if (this->curr_) this->curr_ = this->curr_->prev_;
			else this->curr_ = this->dlist_->back_;
			return old;
		}
		T& operator*()
		{
			return this->curr_->data_;
		}
		const T& operator*()const
		{
			return this->curr_->data_;
		}
	};
	DList();
	~DList();
	DList(const DList& rhs);
	DList& operator=(const DList& rhs);
	DList(DList&& rhs);
	DList& operator=(DList&& rhs);
	iterator insert(iterator it, const T& data);
	iterator search(const T& data);
	iterator erase(iterator it);
	void sort(iterator first, iterator last);
	bool empty() const;
	int size() const;

	iterator begin()
	{
		return iterator(this->front_, this);
	}
	iterator end()
	{
		return iterator(nullptr, this);
	}
	const_iterator cbegin() const
	{
		return const_iterator(this->front_, this);
	}
	const_iterator cend() const
	{
		return const_iterator(nullptr, this);
	}
};

template <typename T>
DList<T>::DList()
{
	front_ = nullptr;
	back_ = nullptr;
}
template <typename T>
DList<T>::~DList()
{
	while (back_)
	{
		erase(end());
	}
}
template <typename T>
DList<T>::DList(const DList& rhs)
{
	this->front_ = this->back_ = nullptr;
	*this = rhs;
}
template <typename T>
DList<T>& DList<T>::operator=(const DList& rhs)
{
	if (this != &rhs)
	{
		while (back_)
		{
			erase(end());
		}
		if (rhs.front_ != nullptr)
		{
			for (auto it = rhs.cbegin(); it != rhs.cend(); it++)
			{
				insert(end(), *it);
			}
		}
	}
	return *this;
}
template <typename T>
DList<T>::DList(DList&& rhs)
{
	this->front_ = this->back_ = nullptr;
	*this = std::move(rhs);

}

template <typename T>
DList<T>& DList<T>::operator=(DList&& rhs)
{
	if (this != &rhs)
	{
		while (back_)
		{
			erase(end());
		}
		this->front_ = rhs.front_;
		this->back_ = rhs.back_;

		rhs.front_ = nullptr;
		rhs.back_ = nullptr;
	}
	return *this;
}


template <typename T>
typename DList<T>::iterator DList<T>::insert(iterator it, const T& data)
{
	Node* nn;
	if (it == begin())
	{
		nn = new Node(data, front_, nullptr);
		if (front_)
		{
			front_->prev_ = nn;
		}
		else
		{
			back_ = nn;
		}
		front_ = nn;
	}
	else if (it == end())
	{
		nn = new Node(data, nullptr, back_);
		if (back_)
		{
			back_->next_ = nn;
		}
		else
		{
			front_ = nn;
		}
		back_ = nn;
	}
	else
	{
		nn = new Node(data, it.curr_, it.curr_->prev_);
		it.curr_->prev_->next_ = nn;
		it.curr_->prev_ = nn;
	}
	return (--it);
}

template <typename T>
typename DList<T>::iterator DList<T>::search(const T& data)
{
	iterator iter = end();
	for (auto it = begin(); it != end(); it++)
	{
		if ((*it) == data)
		{
			iter = it;
			break;
		}
	}
	return iter;
}


template <typename T>
typename DList<T>::iterator DList<T>::erase(iterator it)
{
	iterator iter = ++it;
	it--;
	Node* rm = it.curr_;

	if (it == begin())
	{
		front_ = rm->next_;
		if (front_)
		{
			front_->prev_ = nullptr;
		}
		else
		{
			back_ = nullptr;
		}
	}
	else if (it == (--end()))
	{
		back_ = rm->prev_;
		if (back_)
		{
			back_->next_ = nullptr;
		}
		else
		{
			front_ = nullptr;
		}
	}
	else
	{
		rm->next_->prev_ = rm->prev_;
		rm->prev_->next_ = rm->next_;
	}
	delete rm;
	return iter;
}

template <typename T>
void DList<T>::sort(iterator first, iterator last)
{
	bool looping;
	Node* ptr1;
	Node* ptr2;

	do
	{
		ptr1 = first.curr_;
		looping = 0;

		while (ptr1->next_ != last.curr_)
		{
			ptr2 = ptr1->next_;

			if (ptr1->data_.count_ > ptr2->data_.count_)
			{
				ptr1->next_ = ptr2->next_;
				ptr2->prev_ = ptr1->prev_;

				if (ptr1->prev_) ptr1->prev_->next_ = ptr2;
				if (ptr2->next_) ptr2->next_->prev_ = ptr1;

				ptr1->prev_ = ptr2;
				ptr2->next_ = ptr1;

				if (ptr1 == first.curr_)
				{
					first.curr_ = ptr2;
				}
				if (ptr1->next_ == last.curr_)
				{
					if (last.curr_) last.curr_->prev_ = ptr1;
					else back_ = ptr1;
				}

				looping = true;
			}
			else
			{
				ptr1 = ptr2;
			}
		}
	} while (looping);

	if (!first.curr_->prev_ && front_ != first.curr_) front_ = first.curr_;
}

template <typename T>
bool DList<T>::empty() const
{
	return size() == 0;
}
template <typename T>
int DList<T>::size() const
{
	int count = 0;
	for (auto it = cbegin(); it != cend(); it++)
	{
		++count;
	}
	return count;
}
