#pragma once

#include <cstddef>

template <typename T>
class My_vec
{
public:
    // Constructs a new, empty vector.
    My_vec();
    // Constructs a vector of size `size`, filled with the default value of `T`.
    My_vec(size_t size);
    // Constructs a vector of size `size`, filled with `element`.
    My_vec(size_t size, const T& element);

    // Copy constructor.
    My_vec(const My_vec&);
    // Copy-assignment operator.
    My_vec& operator=(const My_vec&);

    ~My_vec();

    // Is this vector empty?
    bool empty() const;
    // What is the size of this vector?
    size_t size() const;

    // Adds an element at the end of the vector.
    void push_back(const T&);
    // Removes the element at the end of the vector.
    void pop_back();
    // Indexing.
    T& operator[](size_t index);
    // Const indexing.
    const T& operator[](size_t index) const;

private:
    size_t cap_;
    size_t size_;
    T* data_;
};

template <typename T>
My_vec<T>::My_vec()
        : cap_{10}, size_{0}, data_{new T[cap_]}
{ }

template <typename T>
My_vec<T>::My_vec(size_t size)
{
    data_ = new T[size];
    cap_ = size;
    size_ = size;
}

template <typename T>
My_vec<T>::My_vec(size_t size, const T& element)
    : My_vec(size)
{
    for (size_t i = 0; i < cap_; ++i)
        data_[i] = element;
}

template <typename T>
My_vec<T>::My_vec(const My_vec& other)
{
    cap_ = other.size_;
    size_ = cap_;
    data_ = new T[cap_];

    for (int i = 0; i < cap_; ++i)
        data_[i] = other.data_[i];
}

template <typename T>
My_vec<T>& My_vec<T>::operator=(const My_vec& other)
{
    if (cap_ < other.size_) {
        delete [] data_;
        cap_ = other.size_;
        data_ = new T[cap_];
    }

    size_ = other.size_;

    for (size_t i = 0; i < size_; ++i)
        data_[i] = other.data_[i];

    for (size_t i = size_; i < cap_; ++i)
        data_[i] = T{};

    return *this;
}

template <typename T>
My_vec<T>::~My_vec()
{
    delete [] data_;
}

template <typename T>
bool My_vec<T>::empty() const
{
    return size_ == 0;
}

template <typename T>
size_t My_vec<T>::size() const
{
    return size_;
}

template <typename T>
void My_vec<T>::push_back(const T& element)
{
    if (size_ == cap_) {
        cap_ *= 2;
        T* new_data = new T[cap_];

        for (size_t i = 0; i < size_; ++i)
            new_data[i] = data_[i];

        delete [] data_;
        data_ = new_data;
    }

    data_[size_++] = element;
}

template <typename T>
void My_vec<T>::pop_back()
{
    data_[--size_] = T{};
}

template <typename T>
T& My_vec<T>::operator[](size_t index)
{
    return data_[index];
}

template <typename T>
const T& My_vec<T>::operator[](size_t index) const
{
    return data_[index];
}



