#ifndef HEAP_VECTOR_H
#define HEAP_VECTOR_H

#include <cstdlib>
#include <stdexcept>

template<class Key>
class Vector {
public:
    explicit Vector(size_t len = 0);
    explicit Vector(size_t, Key);
    ~Vector();

    size_t size() const;
    bool is_empty() const;
    Key &operator[](size_t i) const;

    void push_back(Key elem);
    void pop_back();
    void reverse();
    void clear();
private:
    Key *ptr;
    size_t len;

};


template<class Key>
void swap(Key &a, Key &b) {
    Key tmp = a;
    a = b;
    b = tmp;
}



template<class Key>
Key max(Key a, Key b) {
    if (a < b) {
        return b;
    }
    else {
        return a;
    }
}

template <class Key>
bool Vector<Key>::is_empty() const {
    return len == 0;
}


template<class Key>
Vector<Key>::Vector(size_t len) {
    this->len = len;
    ptr = new Key[max(len, (size_t)1)];
}

template<class Key>
Vector<Key>::Vector(size_t len, Key key) {
    this->len = len;
    ptr = new Key[max(len, (size_t)1)];
    for (size_t i = 0; i < len; ++i) {
        ptr[i] = key;
    }
}

template <class Key>
Vector<Key>::~Vector() {
    delete []ptr;
}

template <class Key>
size_t Vector<Key>::size() const {
    return len;
}

template<class Key>
Key &Vector<Key>::operator[](size_t index) const {
    if (!(0 <= index && index < len)) {
        throw std::out_of_range("Element accessed by [] is out of range");
    }
    Key &ret = ptr[index];
    return ret;
}

template<class Key>
void Vector<Key>::push_back(Key elem) {
    if (__builtin_popcount(len) == 1) {
        Key *ptr2 = new Key[len * 2];
        for (size_t i = 0; i < len; ++i) {
            ptr2[i] = ptr[i];
        }
        delete[]ptr;
        ptr = ptr2;
    }
    ptr[len] = elem;
    ++len;
}


template <class Key>
void Vector<Key>::pop_back() {
    if (len == 0) {
        throw std::logic_error("Vector instance is empty");
    }
    --len;
}

template <class Key>
void Vector<Key>::reverse() {
    for (int i = 0; i < len / 2; ++i) {
        swap(ptr[i], ptr[len - i - 1]);
    }
}


template <class Key>
void Vector<Key>::clear() {
    len = 0;
}


#endif //HEAP_VECTOR_H
