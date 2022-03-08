#pragma once

#include <iostream>
#include <cassert>

namespace Matrix {

template <typename T> 
class Matrix {
private:
    int size_;
    T** data_;

public: // ctors
    Matrix(int sz = 0) : size_(sz), data_(new T*[size_]) { // default ctor
        assert(data_);
        for(int i = 0; i < size_; i++) {
            data_[i] = new T[size_]{};
            assert(data_[i]);
        }
    }
    //...............................................
    //...............................................
    Matrix(const Matrix& rhs) : size_(rhs.size_), data_(new T*[size_]) { // copy ctor
        assert(data_);

        for(int i = 0; i < size_; i++) {
            data_[i] = new T[size_]{};
            assert(data_[i]);
            std::copy(rhs.data_[i], rhs.data_[i] + size_, data_[i]);
        }
    }
    //...............................................
    //...............................................  
    Matrix(Matrix&& rhs) : size_(rhs.size_), data_(rhs.data_) { //move ctor
        rhs.data_ = nullptr;
    }
    //...............................................
    //............................................... 
    Matrix& operator= (const Matrix& rhs) { //assignment operator
        if(this == &rhs)
            return *this;

        for(int i = 0; i < size_; i++)  
            delete [] data_[i];
        delete [] data_;

        size_ = rhs.size_;
        data_ = new T*[size_];
        for(int i = 0; i < size_; i++)  {
            data_[i] = new T [size_]{};
            assert(data_[i]);
            std::copy(rhs.data_[i], rhs.data_[i] + size_, data_[i]);
        }  
        return *this;
    }
    //...............................................
    Matrix& operator= (Matrix&& rhs) { // move operator
        if(this == &rhs)
            return *this;
        
        std::swap(size_, rhs.size_);
        std::swap(data_, rhs.data_);
        return *this;

    }
    //............................................... 
    struct Proxy {
        
        T* row;
        const T& operator[](int y) const { return row[y]; }
        T& operator[](int y){ return row[y]; }
    };
    
    ~Matrix() {
        for(int i = 0; i < size_; i++) {
            delete [] data_[i];
        }
        delete [] data_;

    };

public: 
    int get_size() const {
        return size_;
    }
    T** get_data() const{
        return data_;
    }
    
    Proxy operator[] (int x) const {
        Proxy tmp {data_[x]};
        return tmp;
    }


}; //end class Matrix

template <typename T>
std::ostream& operator << (std::ostream &out, const Matrix<T> &m) {
    int sz = m.get_size();
    T** data = m.get_data();

    for(int i = 0; i < sz; i++) {
        for(int j = 0; j < sz; j++) {
            out << data[i][j] << " ";
        }
        out << std::endl;
    }
    return out;
}

} // end namespace