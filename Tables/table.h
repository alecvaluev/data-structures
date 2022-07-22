/*************************************************************/
/*                                                           */
/*  Starter file for a2                                      */
/*                                                           */
/*  Author1 Name: Aleksandr Valuev                           */
/*      - class/function list/main author or main checker    */
/*                                                           */
/*************************************************************/

#include <string>
#include <utility>
#include <functional>
#include "mylist.h"

template <class TYPE>
class Table
{
public:
    Table()
    {
    }
    virtual bool update(const std::string& key, const TYPE& value) = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual bool find(const std::string& key, TYPE& value) = 0;
    virtual int numRecords() const = 0;
    virtual bool isEmpty() const = 0;
    virtual int capacity() const = 0;
    virtual ~Table()
    {
    }
};

template <class TYPE>
class SimpleTable :public Table<TYPE>
{

    struct Record
    {
        TYPE data_;
        std::string key_;
        Record(const std::string& key, const TYPE& data)
        {
            key_ = key;
            data_ = data;
        }

    };

    Record** records_;   //the table
    int capacity_;       //capacity of the array


public:
    SimpleTable(int capacity);
    SimpleTable(const SimpleTable& rhs);
    SimpleTable(SimpleTable&& rhs);
    virtual bool update(const std::string& key, const TYPE& value);
    virtual bool remove(const std::string& key);
    virtual bool find(const std::string& key, TYPE& value);
    virtual const SimpleTable& operator=(const SimpleTable& rhs);
    virtual const SimpleTable& operator=(SimpleTable&& rhs);
    virtual ~SimpleTable();
    virtual bool isEmpty() const
    {
        return numRecords() == 0;
    }
    virtual int numRecords() const;
    virtual int capacity() const
    {
        return capacity_;
    }

};

template <class TYPE>
int SimpleTable<TYPE>::numRecords() const
{
    int rc = 0;
    for (int i = 0; records_[i] != nullptr; i++)
    {
        rc++;
    }
    return rc;
}



template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity) : Table<TYPE>()
{
    records_ = new Record * [capacity + 1];
    capacity_ = capacity;
    for (int i = 0; i < capacity_ + 1; i++)
    {
        records_[i] = nullptr;
    }
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& rhs)
{
    records_ = new Record * [rhs.capacity_ + 1];
    capacity_ = rhs.capacity_;
    for (int i = 0; i < capacity_ + 1; i++)
    {
        records_[i] = nullptr;
    }
    for (int i = 0; i < rhs.numRecords(); i++)
    {
        update(rhs.records_[i]->key_, rhs.records_[i]->data_);
    }
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& rhs)
{
    capacity_ = rhs.capacity_;
    records_ = rhs.records_;
    rhs.records_ = nullptr;
    rhs.capacity_ = 0;
}

template <class TYPE>
bool SimpleTable<TYPE>::update(const std::string& key, const TYPE& value)
{
    int idx = -1;
    int sz = numRecords();
    bool rc = true;
    for (int i = 0; i < sz; i++)
    {
        if (records_[i]->key_ == key)
        {
            idx = i;
        }
    }
    if (idx == -1)
    {
        if (sz < capacity_)
        {
            records_[numRecords()] = new Record(key, value);
            for (int i = numRecords() - 1; i > 0 && records_[i]->key_ < records_[i - 1]->key_; i--)
            {
                Record* tmp = records_[i];
                records_[i] = records_[i - 1];
                records_[i - 1] = tmp;
            }
        }
        else
        {
            rc = false;
        }
    }
    else
    {
        records_[idx]->data_ = value;
    }
    return rc;
}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const std::string& key)
{
    int idx = -1;
    for (int i = 0; records_[i] != nullptr; i++)
    {
        if (records_[i]->key_ == key)
        {
            idx = i;
        }
    }
    if (idx != -1)
    {
        delete records_[idx];

        for (int i = idx; i < numRecords() - 1; i++)
        {
            records_[i] = records_[i + 1];
        }
        records_[numRecords() - 1] = nullptr;
        return true;
    }
    else
    {
        return false;
    }
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const std::string& key, TYPE& value)
{
    int idx = -1;
    for (int i = 0; i < numRecords(); i++)
    {
        if (records_[i]->key_ == key)
        {
            idx = i;
        }
    }
    if (idx == -1)
        return false;
    else
    {
        value = records_[idx]->data_;
        return true;
    }
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& rhs)
{
    if (this != &rhs)
    {
        if (records_)
        {
            while (numRecords() != 0)
            {
                remove(records_[0]->key_);
            }
            delete[] records_;
        }
        records_ = new Record * [rhs.capacity_ + 1];
        capacity_ = rhs.capacity_;
        for (int i = 0; i < capacity_; i++)
        {
            records_[i] = nullptr;
        }
        for (int i = 0; i < rhs.numRecords(); i++)
        {
            update(rhs.records_[i]->key_, rhs.records_[i]->data_);
        }

    }
    return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& rhs)
{
    if (records_)
    {
        while (numRecords() != 0)
        {
            remove(records_[0]->key_);
        }
        delete[] records_;
    }
    records_ = rhs.records_;
    capacity_ = rhs.capacity_;
    rhs.records_ = nullptr;
    rhs.capacity_ = 0;

    return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable()
{
    if (records_)
    {
        int sz = numRecords();
        for (int i = 0; i < sz; i++)
        {
            remove(records_[0]->key_);
        }
        delete[] records_;
    }
}



/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/******************************* ChainingTable ***********************************/


template <class TYPE>
class ChainingTable :public Table<TYPE>
{
    struct Record
    {
        TYPE data_;
        std::string key_;
        Record()
        {
        }
        Record(const std::string& key, const TYPE& data)
        {
            key_ = key;
            data_ = data;
        }
        bool operator==(const Record& rc) const
        {
            return key_ == rc.key_;
        }
        bool operator!=(const Record& rc) const
        {
            return key_ != rc.key_;
        }
        bool operator<(const Record& rc) const
        {
            return key_ < rc.key_;
        }
    };

    DList<Record>** records_{ nullptr };
    int capacity_{ 0 };
    int size_{ 0 }; //keeps number of records

    size_t getHashIdx(const std::string key);
    void clearTable();
public:
    ChainingTable(int maxExpected);
    ChainingTable(const ChainingTable& other);
    ChainingTable(ChainingTable&& other);
    double loadFactor() const;
    virtual bool update(const std::string& key, const TYPE& value);
    virtual bool remove(const std::string& key);
    virtual bool find(const std::string& key, TYPE& value);
    virtual const ChainingTable& operator=(const ChainingTable& other);
    virtual const ChainingTable& operator=(ChainingTable&& other);
    virtual ~ChainingTable();
    virtual bool isEmpty() const;
    virtual int numRecords() const;
    virtual int capacity() const;

};
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(int capacity) : Table<TYPE>()
{
    capacity_ = capacity;
    records_ = new DList<Record> * [capacity_];
    for (int i = 0; i < capacity_; i++)
    {
        records_[i] = nullptr;
    }
}

template <class TYPE>
ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other)
{
    *this = other;
}
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other)
{
    *this = std::move(other);
}

/*the function takes two parameters - a key and a value. It generates the 
*position using hash, creates a new list for records and inserts the record if 
*there is none in the table, updates the record if there is already a record 
*with the same key but different value, or do nothing if the records with the 
*same key and value is in the table. In cases when new record is inserted or 
*updated the function returns true, otherwise in cases when no updates happens, 
*it returns false.
*/
template <class TYPE>
bool ChainingTable<TYPE>::update(const std::string& key, const TYPE& value)
{
    bool result = true;
    Record newRecord(key, value);

    size_t idx = getHashIdx(key);

    if (records_[idx])
    {
        //the record exists, find it's iterator
        typename DList<Record>::iterator it = records_[idx]->search(newRecord);

        if (it != records_[idx]->end())
        {
            if ((*it).data_ != value)
            {
                //different data, update
                (*it).data_ = value;
            }
            else
            {
                //same data, do nothing
                result = false;
            }
        }
        else
        {
            //the record not found, insert one
            records_[idx]->insert(records_[idx]->end(), newRecord);
            size_++;
        }
    }
    else
    {
        //nothing is here, create a new list and insert
        records_[idx] = new DList<Record>();
        records_[idx]->insert(records_[idx]->end(), newRecord);
        size_++;
    }
    return result;
}

/*the function takes a key of the records it needs to remove and searches the 
* table, if the record found, the function erases it and then checks if the 
* list is empty and needs to be deleted to save memory. If the record was 
* deleted successfully, it returns true, otherwise false.
*/
template <class TYPE>
bool ChainingTable<TYPE>::remove(const std::string& key)
{
    bool removed = false;
    size_t idx = getHashIdx(key);

    if (records_[idx])
    {
        //list exists
        TYPE value;
        find(key, value);
        Record record(key, value);
        //find the record
        typename DList<Record>::iterator it = records_[idx]->search(record);

        if (it != records_[idx]->end())
        {
            //record found, erase it
            records_[idx]->erase(it);
            removed = true;
            size_--;

            if (records_[idx]->size() == 0)
            {
                //if there're no more records in the list, delete it
                delete records_[idx];
                records_[idx] = nullptr;
            }
        }
    }
    return removed;
}

/*the function uses the key passed as a parameter to find the record and if it 
* finds it, it assigns the value to the second parameter and returns true
* otherwise function only returns false as for not found
*/
template <class TYPE>
bool ChainingTable<TYPE>::find(const std::string& key, TYPE& value)
{
    bool found = false;
    size_t idx = getHashIdx(key);

    if (records_[idx])
    {
        //list exists, find the records in it
        Record record(key, value);
        typename DList<Record>::iterator it = records_[idx]->search(record);

        if (it != records_[idx]->end())
        {
            //the record found, copy value
            value = (*it).data_;
            found = true;
        }
    }
    return found;
}

//the assigning operator copies data from the table to the current table
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other)
{
    if (this != &other)
    {
        clearTable();
        //copy 
        capacity_ = other.capacity_;
        size_ = other.size_;
        if(capacity_ > 0) records_ = new DList<Record> * [capacity_];
        if (other.records_)
        {
            //other has some records
            for (int i = 0; i < capacity(); i++)
            {
                if (other.records_[i])
                {
                    records_[i] = new DList<Record>(*other.records_[i]);
                }
                else
                {
                    records_[i] = nullptr;
                }
            }
        }
    }
    return *this;
}

//the function moves the data and records from a table to the current table
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other)
{
    if (this != &other)
    {
        capacity_ = other.capacity_;
        records_ = other.records_;
        size_ = other.size_;
        other.capacity_ = 0;
        other.size_ = 0;
        other.records_ = nullptr;
    }
    return *this;
}

template <class TYPE>
ChainingTable<TYPE>::~ChainingTable()
{
    clearTable();
}

template <class TYPE>
bool ChainingTable<TYPE>::isEmpty() const
{
    return numRecords() == 0;
}

//returns number of records in the table by counting how many records in each list
template <class TYPE>
int ChainingTable<TYPE>::numRecords() const
{
    /*int nRecords = 0;
    if (records_)
    {
        for (int i = 0; i < capacity_; i++)
        {
            if (records_[i])  nRecords += records_[i]->size();
        }
    }
    return nRecords;
    */
    return size_;
}
//returns capasity of the table
template <class TYPE>
int ChainingTable<TYPE>::capacity() const
{
    return capacity_;
}

//returns load factor of the table
template <class TYPE>
double ChainingTable<TYPE>::loadFactor() const
{
    return (double)numRecords() / capacity_;
}

//calculate the hash index 
template <class TYPE>
size_t ChainingTable<TYPE>::getHashIdx(const std::string key)
{
    size_t hash = std::hash<std::string>{}(key);
    return hash % capacity();
}

//clear the table by deleting all data
template <class TYPE>
void ChainingTable<TYPE>::clearTable()
{
    if (records_)
    {
        for (int i = 0; i < capacity(); i++)
        {
            if (records_[i] != nullptr)
            {
                delete records_[i];
            }
        }
        delete[] records_;
        records_ = nullptr;
    }
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/******************************* LPTable *****************************************/


template <class TYPE>
class LPTable :public Table<TYPE>
{
    struct Record
    {
        TYPE data_;
        std::string key_;
        Record()
        {
        }
        Record(const std::string& key, const TYPE& data)
        {
            key_ = key;
            data_ = data;
        }
    };

    Record** records_{ nullptr };
    int capacity_{ 0 };
    int size_{ 0 };

    size_t getHashIdx(const std::string key, const size_t position);
    void clearTable();
public:
    LPTable(int maxExpected);
    LPTable(const LPTable& other);
    LPTable(LPTable&& other);
    double loadFactor() const;
    virtual bool update(const std::string& key, const TYPE& value);
    virtual bool remove(const std::string& key);
    virtual bool find(const std::string& key, TYPE& value);
    virtual const LPTable& operator=(const LPTable& other);
    virtual const LPTable& operator=(LPTable&& other);
    virtual ~LPTable();
    virtual bool isEmpty() const;
    virtual int numRecords() const;
    virtual int capacity() const;

};
template <class TYPE>
LPTable<TYPE>::LPTable(int capacity) : Table<TYPE>()
{
    capacity_ = capacity;
    records_ = new Record * [capacity_];
    for (int i = 0; i < capacity_; i++)
    {
        records_[i] = nullptr;
    }
}

template <class TYPE>
LPTable<TYPE>::LPTable(const LPTable<TYPE>& other)
{
    *this = other;
}
template <class TYPE>
LPTable<TYPE>::LPTable(LPTable<TYPE>&& other)
{
    *this = std::move(other);
}
/*the function takes two parameters - a key and a value. It generates the 
* position using hash and creates a new records if there is no in the table, 
* updates if there is already a record with the same key but different value, 
* or do nothing if the recordswith the same key and value is in the table. 
* In cases when new record is created or updated the function returns true, 
* otherwise in cases when no updates happens, it returns false.
*/
template <class TYPE>
bool LPTable<TYPE>::update(const std::string& key, const TYPE& value)
{
    bool updated = true;
    size_t position = 0;
    size_t idx = getHashIdx(key, position);

    while (records_[idx] && records_[idx]->key_ != key && position < capacity())
    {
        //there is an object but with different key, try the next record
        position++;
        idx = getHashIdx(key, position);
    }
    if (position == capacity()) updated = false;
    else
    {
        //the spot is empty, create a new record
        if (!records_[idx]) {
            records_[idx] = new Record(key, value);
            size_++;
        }
        else
        {
            //record found, update if needed
            if (records_[idx]->data_ == value) updated = false;
            else records_[idx]->data_ = value;
        }
    }
    return updated;
}

/*the function takes a key of the records it needs to remove and searches the 
* table, if the record found, the function erases it and then shifts the 
* cluster of records after the deleted record. If the record was deleted 
* successfully, it returns true, otherwise false.
*/
template <class TYPE>
bool LPTable<TYPE>::remove(const std::string& key)
{
    bool removed = false;

    if (records_)
    {
        size_t position = 0;
        size_t idx = getHashIdx(key, position);

        while (records_[idx] && records_[idx]->key_ != key && position < capacity())
        {
            //there is an object but with different key, try the next record
            position++;
            idx = getHashIdx(key, position);
        }
        if (position < capacity() && records_[idx] && records_[idx]->key_ == key)
        {
            //record with the same key found, delete it
            delete records_[idx];
            records_[idx] = nullptr;
            removed = true;
            size_--;

            //check and move records in the cluster
            idx++;
            size_t capacity = capacity_ - 1;
            while (records_[(idx % capacity)] != nullptr)
            {
                //there is a record, try to move it
                if (update(records_[(idx % capacity)]->key_, records_[(idx % capacity)]->data_))
                {
                    //record moved and copied, empty the spot
                    delete records_[(idx % capacity)];
                    records_[(idx % capacity)] = nullptr;
                }
                idx++;
            }
        }
    }
    return removed;
}

/*the function uses the key passed as a parameter to find the record and if it 
* finds it, it assigns the value to the second parameter and returns true
* otherwise function only returns false as for not found
*/
template <class TYPE>
bool LPTable<TYPE>::find(const std::string& key, TYPE& value)
{
    bool found = false;
    size_t position = 0;
    size_t idx = getHashIdx(key, position);

    while (records_[idx] && position != capacity())
    {
        //record exists
        if (records_[idx]->key_ == key)
        {
            //the record found
            value = records_[idx]->data_;
            found = true;
            break;
        }
        //check next record
        position++;
        idx = getHashIdx(key, position);
    }
    return found;
}

//the assigning operator copies data from the table to the current table
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(const LPTable<TYPE>& other)
{
    if (this != &other)
    {
        clearTable();
        capacity_ = other.capacity_;
        size_ = other.size_;
        records_ = new Record * [capacity_];

        if (other.records_)
        {
            for (int i = 0; i < capacity_; i++)
            {
                if (other.records_[i])
                {
                    records_[i] = new Record(other.records_[i]->key_, other.records_[i]->data_);
                }
                else
                {
                    records_[i] = nullptr;
                }
            }
        }
    }
    return *this;
}

//the function moves data and records from one table to the current
template <class TYPE>
const LPTable<TYPE>& LPTable<TYPE>::operator=(LPTable<TYPE>&& other)
{
    records_ = other.records_;
    capacity_ = other.capacity();
    size_ = other.size_;
    other.records_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
    return *this;
}

template <class TYPE>
LPTable<TYPE>::~LPTable()
{
    clearTable();
}

//the function compares if the number of records in the table is 0 or not. If 
//it is then it returns true, false otherwise.
template <class TYPE>
bool LPTable<TYPE>::isEmpty() const
{
    return numRecords() == 0;
}

//returns number of records in the table by counting each record
template <class TYPE>
int LPTable<TYPE>::numRecords() const
{
    /*int nRecords = 0;
    if (records_)
    {
        for (int i = 0; i < capacity(); i++)
        {
            if (records_[i])  nRecords++;
        }
    }
    return nRecords;*/
    return size_;
}
//returns capacity of the table
template <class TYPE>
int LPTable<TYPE>::capacity() const
{
    return capacity_;
}
//returns load factor of the table
template <class TYPE>
double LPTable<TYPE>::loadFactor() const
{
    return (double)numRecords() / capacity_;
}

/*The function takes two parameter - string for hashing and position, it 
* creates the hash value and then calculates the position in the table
* and shift for higher index position if there is 
*/
template <class TYPE>
size_t LPTable<TYPE>::getHashIdx(const std::string key, const size_t position)
{
    size_t hash = std::hash<std::string>{}(key);
    return (hash + position) % (capacity() - 1);
}

// The function deletes all data records within the table including the pointer 
//to the table 
template <class TYPE>
void LPTable<TYPE>::clearTable()
{
    if (records_)
    {
        for (int i = 0; i < capacity(); i++)
        {
            if (records_[i]) delete records_[i];
        }
        delete[] records_;
        records_ = nullptr;
    }
}
