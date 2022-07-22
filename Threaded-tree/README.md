# Threaded Binary Search Tree
The basic class declarations can be found in the file threadedtree.h

Write the following binary search tree called ThreadedTree, along with const_iterator and iterator for the binary search tree. This type allows you to iterate through the tree through an iterator with the help of threads that allow you to go through nodes in an in-order manner

A threaded binary search tree is a binary search tree where the pointers that are normally nullptrs would instead be used to point at the next smallest (left pointer) or the next biggest (right pointer) node in the tree. These pointers are called threads. These threads make it easier to iterate through the tree.

The data type of the data stored in this tree (T) supports the following operator:

* ==
* !=
* =
* <
* default construction
This means that you can use any of the above operators/features on the data stored in the nodes of your threaded binary search tree. You do not need to implement these operators. It is up to the program using the linked list to implement these operators for the data type.

A node in a threaded tree is different from that of a regular bst node as it uses what would normally be nullptrs to point at previous and next nodes for an inorder traversal of a tree. As the left/right pointers can have different uses (either as regular left/right pointers or as threads), a flag exists for each pointer to indicate whether or not the pointer is being used as threads or not threads (normal links)

Here is a diagram of how threaded Tree insertions work. links changed during insertion of new nodes are shown as dotted lines. Red links (always curved) are threads, black links are regular links. 

The ThreadedTree must have the following member functions:
```
ThreadedTree();
```
constructor, creates empty ThreadedTree
```
iterator begin();
const_iterator cbegin() const;
```
returns the appropriate iterator to the smallest node in the tree
```
iterator end();
const_iterator cend() const;
```
returns the appropriate iterator to the node after the largest node in the tree.
```
void insert(const T& data)
```
The function inserts the data into the tree. You may assume there will not be duplicates added.
```
iterator find(const T& key);
const_iterator find(const T& key) const;
```
this function accepts a value, key and returns an iterator/const_iterator to the node containing that value. If key is not found, function returns end()/cend();
```
int size() const;
```
This function returns the number of values stored in the tree
```
bool empty() const;
```
This function returns true if the tree has no values stored, false otherwise
```
~ThreadedTree()
```
destructor for ThreadedTree. All nodes must be deallocated

### const_iterator and iterator
The const_iterator/iterators of a ThreadedTree allows for the inorder traversal of the tree. That is, the iterators go through the nodes from smallest to biggest.

The const_iterator and iterators must support the following operations:
```
bool operator==(const_iterator rhs); - function returns true if rhs and current object refer to the same node
bool operator!=(const_iterator rhs); - function returns true if rhs and current object does not refer to the same node
```
```
iterator operator++();           //prefix ++
iterator operator++(int);        //postfix ++
const_iterator operator++();     //prefix ++
const_iterator operator++(int);  //postfix ++
```
iterator advances to next biggest value in the tree if the iterator is not currently at end(). The iterator returned depends if it is prefix or postfix. prefix operator returns iterator to current node. postfix operator returns iterator to node before the increment.
```
iterator operator--();           //prefix --
iterator operator--(int);        //postfix --
const_iterator operator--();     //prefix --
const_iterator operator--(int);  //postfix --
```
iterator goes to the next smallest node in the tree. The iterator returned depends if it is prefix or postfix. prefix operator returns iterator to current node. postfix operator returns iterator to node before the decrement.
```
const T& operator*() const;   //in const_iterator
const T& operator*() const;   //in iterator
```
returns a const reference to data in the node referred to by the iterator.
```
T& operator*();  
```
returns a reference to data in the node referred to by the iterator.
