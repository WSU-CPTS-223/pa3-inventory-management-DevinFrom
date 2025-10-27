#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP

#include <cstddef>
#include <stdexcept>

template <typename T>
class DynArray 
{
public:
    DynArray() : _data(nullptr), _size(0), _capacity(0) 
    {}
    explicit DynArray(size_t cap) : _data(nullptr), _size(0), _capacity(0) { reserve(cap); }
    ~DynArray() { delete[] _data; }

    DynArray(const DynArray& other) : _data(nullptr), _size(0), _capacity(0) {
        reserve(other._size);
        for (size_t i = 0; i < other._size; ++i) _data[i] = other._data[i];
        _size = other._size;
    }
    DynArray& operator=(const DynArray& other) 
    {
        if (this != &other) {
            delete[] _data; _data = nullptr; _size = 0; _capacity = 0;
            reserve(other._size);
            for (size_t i = 0; i < other._size; ++i) _data[i] = other._data[i];
            _size = other._size;
        }
        return *this;
    }

    void push_back(const T& val) {
        if (_size + 1 > _capacity) reserve(_capacity == 0 ? 8 : _capacity * 2);
        _data[_size++] = val;
    }

    void clear() { _size = 0; }

    void reserve(size_t newcap)
     {
        if (newcap <= _capacity) return;
        T* nd = new T[newcap];
        for (size_t i = 0; i < _size; ++i) nd[i] = _data[i];
        delete[] _data;
        _data = nd; _capacity = newcap;
    }

    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }

    T& operator[](size_t idx) 
    {
        if (idx >= _size) throw std::out_of_range("DynArray index out of range");
        return _data[idx];
    }
    const T& operator[](size_t idx) const 
    {
        if (idx >= _size) throw std::out_of_range("DynArray index out of range");
        return _data[idx];
    }

    T* data() { return _data; }
    const T* data() const { return _data; }

private:
    T* _data;
    size_t _size;
    size_t _capacity;
};

#endif 
