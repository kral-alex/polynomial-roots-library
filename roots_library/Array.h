//
// Created by Alex Král on 07.04.2022.
//

#ifndef ROOTS___ARRAY_H
#define ROOTS___ARRAY_H

#include <iostream>

template<class T> class Array;
template<class T> std::ostream& operator<< (std::ostream&, const Array<T>&);


template<class T>
class Array{
    T* array_;
    unsigned len_;
    bool owner_ = true;

    class Proxy {
        friend Array;
        T* pointer;
        explicit Proxy(T* ptr): pointer(ptr) {};

    public:
        Proxy& operator= (T val) {*pointer = val; return *this;}
        operator T() {return *pointer;} // NOLINT(google-explicit-constructor)
        // must be able to implicitly convert for subscripting to work properly
    };

public:

    explicit Array(int n): len_(n) {
        array_ = static_cast<T*>(malloc(len_ * sizeof(T)));
        if (array_ == NULL) throw std::runtime_error("malloc failed");
    }

    Array(const Array& a): len_(a.len_) {
        array_ = static_cast<T*>(malloc(len_ * sizeof(T)));
        memcpy(array_, a.array_, sizeof(T) * len_);
    }

    Array& operator= (const Array& a) =delete;

    Array& slice(int start, int end) const {
        if (start < 0) start = len_ - start;
        if (end < 0) end = len_ - end;
        Array a = Array(this);
        a.array_ += start;
        a.len_ -= start + end;
        return *a;
    }

    const Array& slice(int start, int end) {
        if (start < 0) start = len_ - start;
        if (end < 0) end = len_ - end;
        Array a;
        a.owner_ = false;
        a.array_ = array_ + start;
        a.len_ = len_ - (start + end);
        return *a;
    }

    ~Array() {
        if (owner_ && array_ != NULL)
            free(array_);
    }

    Proxy operator[] (int i) const {
        if (i >= len_) {
            throw std::runtime_error("index you are trying to access is out of range");
        }
        return Proxy(array_ + i);
    }

    T* array() {return array_;}

    const T* array() const {return array_;} //functions with const pointer parameters can only take this overload

    [[nodiscard]] int len() const {return len_;}  // non-discard prevents function call with return value discarded

    friend std::ostream& operator<< (std::ostream& os, const Array& array) {
        for (int i = 0; i < array.len_; i++) {
            os << i << ": " << array.array_[i] << '\n';
        }
        return os;
    }

};


#endif //ROOTS___ARRAY_H
