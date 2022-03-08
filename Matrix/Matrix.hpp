#pragma once

#include <iostream>
#include <cassert>
namespace Matrix {

template <typename T>
class Matrix {
private:
    int size_;
    T** data_;

public:
    Matrix(int sz = 0) : size_(sz), data_(new T* [size_]) { //default ctor
        assert(data_);
        for(int i = 0; i < size_; i++) {
            data_[i] = new T [size_]{};
            assert(data_[i]);
        }
    }
    Matrix(int sz, T** data) : size_(sz), data_(data) {}
    //...............................................
    //...............................................
    Matrix(const Matrix& rhs) : size_(rhs.size_), data_(new T* [size_])  {// copy ctor
        assert(data_);
        
        for(int i = 0; i < size_; i++)  {
            data_[i] = new T [size_]{};
            assert(data_[i]);
            std::copy(rhs.data_[i], rhs.data_[i] + size_, data_[i]);
        }  
    }
    //...............................................
    //...............................................
    Matrix(Matrix&& rhs) : size_(rhs.size_), data_(rhs.data_) { //Move ctor
        rhs.data_ = nullptr;
    }
    //...............................................
    //...............................................
    Matrix& operator= (const Matrix& rhs) { //assignment operator
        if (this == &rhs)
            return *this;

        for(int i = 0; i < size_; i++)
            delete [] data_[i];
        delete [] data_;

        size_ = rhs.size_;
        data_ = new T* [size_]{};
        for(int i = 0; i < size_; i++)  {
            data_[i] = new T [size_]{};
            assert(data_[i]);
            std::copy(rhs.data_[i], rhs.data_[i] + size_, data_[i]);
        }  
        return *this;
    }
    //...............................................
    //...............................................
    Matrix& operator= (Matrix&& rhs) { //move operator
        if(this == &rhs)
            return *this;

        std::swap(size_, rhs.size_);
        std::swap(data_, rhs.data_);

        return *this;
    }
    //...............................................
    //...............................................
    ~Matrix() { // dtor
        for(int i = 0; i < size_; i++)
            delete [] data_[i];
        delete [] data_;
    }
    //...............................................
    //...............................................
public:
    void determinant() {
        
    }

    void print()
    {
        for(int i = 0; i < size_; i++) {
            for(int j = 0; j < size_; j++) {
                std::cout << data_[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    //...............................................
    //...............................................
    int get_size() const {
        return size_;
    }
    //...............................................
    //...............................................
    T** get_data() const {
        return data_;
    }
    //...............................................
    //...............................................
    void swap_columns(int i, int j) {
        if(i >= size_ || j >= size_ || i < 0 || j < 0)
            return;
        std::swap(data_[i], data_[j]);
    }
    //...............................................
    //...............................................
    void swap_rows(int i, int j) {
        if(i >= size_ || j >= size_ || i < 0 || j < 0)
            return;
        for(int k = 0; k < size_; k++)
            std::swap(data_[k][i], data_[k][j]);
    }
};



} // end namespace
