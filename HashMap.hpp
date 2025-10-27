#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <functional>
#include <cstddef>
#include <string>


template <typename Key, typename Value>
class HashMap 
{
public:

/***************************************************************************************
Function: HashMap::HashMap                                                             *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Acts as the constructor for the hash map needed                           *
Input parameters: N/A                                                                  *
Returns: N/A                                                                           *
Preconditions: None                                                                    *
Postconditions: Table can accept inserts                                               *
***************************************************************************************/
    HashMap() : _keys(nullptr), _values(nullptr), _occupied(nullptr), _deleted(nullptr),
                _capacity(0), _size(0) { reserve(16); }


/***************************************************************************************
Function: HashMap::~HashMap                                                            *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Acts as the deconstructor for the hash map needed                         *
Input parameters: N/A                                                                  *
Returns: N/A                                                                           *
Preconditions: Table is constucted                                                     *
Postconditions: All mappings are removed                                               *
***************************************************************************************/               
    ~HashMap() 
    {
        delete[] _keys;
        delete[] _values;
        delete[] _occupied;
        delete[] _deleted;
    }

    HashMap(const HashMap& other) : HashMap() 
    {
        reserve(other._capacity);
        for (size_t i = 0; i < other._capacity; ++i) {
            if (other._occupied[i] && !other._deleted[i])
             {
                insert(other._keys[i], other._values[i]);
            }
        }
    }

/***************************************************************************************
Function: HashMap::Operator                                                            *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Assigns the table from another by clearing state                          *
Input parameters: const HashMap& other                                                 *
Returns: HashMap&                                                                      *
Preconditions: None                                                                    *
Postconditions: Will have the same pairs as other table                                *
***************************************************************************************/
    HashMap& operator=(const HashMap& other) 
    {
        if (this != &other)
         {
            clear();
            reserve(other._capacity);
            for (size_t i = 0; i < other._capacity; ++i)
             {
                if (other._occupied[i] && !other._deleted[i])
                 {
                    insert(other._keys[i], other._values[i]);
                }
            }
        }
        return *this;
    }

/***************************************************************************************
Function: HashMap::Clear                                                               *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Marks all slots as empty                                                  *
Input parameters: N/A                                                                  *
Returns: N/A                                                                           *
Preconditions: Table is constructed                                                    *
Postconditions: All mappings removed                                                   *
***************************************************************************************/    
    void clear() 
    {
        for (size_t i = 0; i < _capacity; ++i)
         {
            _occupied[i] = false; _deleted[i] = false;
        }
        _size = 0;
    }

/***************************************************************************************
Function: HashMap::size                                                                *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Shows number of entries in table                                          *
Input parameters: size_t                                                               *
Returns: N/A                                                                           *
Preconditions: Table is constructed                                                    *
Postconditions: N/A                                                                    *
***************************************************************************************/
    size_t size() const { return _size; }

/***************************************************************************************
Function: HashMap::Capacity                                                            *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Shows number of slots available                                           *
Input parameters: N/A                                                                  *
Returns: size_t                                                                        *
Preconditions: Table is constructed                                                    *
Postconditions: N/A                                                                    *
***************************************************************************************/  
    size_t capacity() const { return _capacity; }

/***************************************************************************************
Function: HashMap::reserve                                                             *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Grows backing array and rehashes                                          *
Input parameters: size_t                                                               *
Returns: N/A                                                                           *
Preconditions: Table at current capacity                                               *
Postconditions: Load factor decreases                                                  *
***************************************************************************************/    
    void reserve(size_t newcap) 
    {
        if (newcap <= _capacity) return;
        Key* nkeys = new Key[newcap];
        Value* nvals = new Value[newcap];
        bool* nocc = new bool[newcap];
        bool* ndel = new bool[newcap];
        for (size_t i = 0; i < newcap; ++i) { nocc[i] = false; ndel[i] = false; }

        // rehash existing entries
        if (_capacity > 0) {
            for (size_t i = 0; i < _capacity; ++i) 
            {
                if (_occupied[i] && !_deleted[i]) {
                    insert_into_arrays(nkeys, nvals, nocc, newcap, _keys[i], _values[i]);
                }
            }
            delete[] _keys; delete[] _values; delete[] _occupied; delete[] _deleted;
        }
        _keys = nkeys; _values = nvals; _occupied = nocc; _deleted = ndel; _capacity = newcap;
    }

/***************************************************************************************
Function: HashMap::insert                                                              *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Inserts or replaces key value pairs                                       *
Input parameters: const Key& k, const Value& v                                         *
Returns: bool                                                                          *
Preconditions: Table is constructed                                                    *
Postconditions: Table contains mapping                                                 *
***************************************************************************************/
    bool insert(const Key& k, const Value& v) 
    {
        if ((_size + 1) * 100 / _capacity > 70) reserve(_capacity * 2);

        std::hash<Key> hasher;
        size_t idx = hasher(k) % _capacity;
        size_t start = idx;
        size_t first_tomb = npos;

        while (_occupied[idx]) {
            if (!_deleted[idx] && _keys[idx] == k) 
            {
                _values[idx] = v; // replace
                return false;
            }
            if (_deleted[idx] && first_tomb == npos) first_tomb = idx;
            idx = (idx + 1) % _capacity;
            if (idx == start) break;
        }
        size_t dest = (first_tomb != npos) ? first_tomb : idx;
        _keys[dest] = k;
        _values[dest] = v;
        _occupied[dest] = true;
        _deleted[dest] = false;
        ++_size;
        return true;
    }

/***************************************************************************************
Function: HashMap::contains                                                            *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Checks if a key exists in table                                           *
Input parameters: const Key& k                                                         *
Returns: bool                                                                          *
Preconditions: Table is constructed                                                    *
Postconditions: N/A                                                                    *
***************************************************************************************/   
    bool contains(const Key& k) const
     {
        return find_index(k) != npos;
    }
/***************************************************************************************
Function: HashMap::find                                                                *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Looks up key and returns an associated pointer                            *
Input parameters: const Key& k                                                         *
Returns: value*                                                                        *
Preconditions: Table is constructed                                                    *
Postconditions: N/A                                                                    *
***************************************************************************************/   
    Value* find(const Key& k)
     {
        size_t idx = find_index(k);
        return idx == npos ? nullptr : &_values[idx];
    }
    const Value* find(const Key& k) const 
    {
        size_t idx = find_index(k);
        return idx == npos ? nullptr : &_values[idx];
    }
/***************************************************************************************
Function: HashMap::erase                                                               *
Date Created: 10/26/2025                                                               *
Date Last Modified: 10/26/2025                                                         *
Description: Removes a key value pair                                                  *
Input parameters: const Key& k                                                         *
Returns: bool                                                                          *
Preconditions: Table is constructed                                                    *
Postconditions: map with deleted pair                                                  *
***************************************************************************************/   
    bool erase(const Key& k)
     {
        size_t idx = find_index(k);
        if (idx == npos) return false;
        _deleted[idx] = true; --_size; 
        return true;
    }

private:
    static const size_t npos = static_cast<size_t>(-1);

    size_t find_index(const Key& k) const
     {
        std::hash<Key> hasher;
        size_t idx = hasher(k) % _capacity;
        size_t start = idx;
        while (_occupied[idx]) {
            if (!_deleted[idx] && _keys[idx] == k) return idx;
            idx = (idx + 1) % _capacity;
            if (idx == start) break;
        }
        return npos;
    }

    static void insert_into_arrays(Key* keys, Value* vals, bool* occ, size_t cap,
                                   const Key& k, const Value& v) 
                                   
                                   {
        std::hash<Key> hasher;
        size_t idx = hasher(k) % cap;
        while (occ[idx]) idx = (idx + 1) % cap;
        keys[idx] = k; vals[idx] = v; occ[idx] = true;
    }

    Key* _keys;
    Value* _values;
    bool* _occupied;
    bool* _deleted;
    size_t _capacity;
    size_t _size;
};

#endif 
