#ifndef HEAP_HEAP_H
#define HEAP_HEAP_H


#include "Vector.h"
#include <cstdlib>
#include <cmath>


template<class Key>
class Heap {
private:
    class Node;

public:

    class Pointer {
        friend Heap<Key>;
    private:
        Node *ptr;
        explicit Pointer(Node *ptr_);
    public:
        Pointer();
        Key getKey();
    };

    Heap();

    template <class Iterator>
    Heap(Iterator begin, Iterator end);

    bool is_empty() const;
    Pointer insert(Key);
    void erase(Pointer);
    Key extract_min();
    void change(Pointer, Key);
    Key get_min() const;
    void optimize(size_t, size_t);
    void merge(Heap &otherHeap);
private:

    class Node {
        friend Heap<Key>;
    private:
        Key key;
        size_t index;
        Node(Key key_, size_t index_ = 0);
    };

    Vector<Node*> nodes;
    // amount of children of vertex
    int k;

    void swap_nodes(size_t i, size_t j);
    void siftUp(size_t index);
    void siftDown(size_t index);

    double func_support(double);
    double func_optimized(double, int, int);
};



template <class Key>
Heap<Key>::Pointer::Pointer() {
    ptr = nullptr;
}


template <class Key>
Heap<Key>::Pointer::Pointer(Node *ptr_) {
    ptr = ptr_;
}


template <class Key>
Key Heap<Key>::Pointer::getKey() {
    return ptr->key;
}


template <class Key>
Heap<Key>::Heap() {
    k = 2;
}


template <class Key>
bool Heap<Key>::is_empty() const {
    return nodes.is_empty();
}


template<class Key>
typename Heap<Key>::Pointer Heap<Key>::insert(Key key) {
    Node *nw = new Node(key, nodes.size());
    nodes.push_back(nw);
    siftUp(nodes.size() - 1);
    return Pointer(nw);
}


template<class Key>
Key Heap<Key>::get_min() const {
    if (is_empty()) {
        throw std::logic_error("Heap instance is empty");
    }
    return nodes[0]->key;
}


template<class Key>
Key Heap<Key>::extract_min() {
    if (is_empty()) {
        throw std::logic_error("Heap instance is empty");
    }

    Key return_value = nodes[0]->key;

    swap_nodes(0, nodes.size() - 1);
    delete nodes[nodes.size() - 1];
    nodes.pop_back();
    siftDown(0);

    return return_value;
}


template <class Key>
void Heap<Key>::erase(Heap<Key>::Pointer ptr) {
    size_t index = ptr.ptr->index;
    swap_nodes(index, nodes.size() - 1);
    nodes.pop_back();
    // even if index is out of range now we can use siftDown - it will do nothing in this case
    siftDown(index);
}


template <class Key>
void Heap<Key>::change(Pointer ptr, Key key) {
    ptr.ptr->key = key;
    siftUp(ptr.ptr->index);
    siftDown(ptr.ptr->index);
}


template <class Key>
template<class Iterator>
Heap<Key>::Heap(Iterator begin, Iterator end) {
    Heap<Key>();
    while (begin != end) {
        insert(*begin);
        ++begin;
    }
    k = 2;
}


template <class Key>
void Heap<Key>::optimize(size_t insertCount, size_t extractCount) {
    if (extractCount == 0) {
        k = insertCount + 10;
        return;
    }

    // in order to minimize amount of operations, we should minimize:
    // -> insertCount * log(k, n) + extractCount * log(k, n) * k == ...
    // ... == ln(n) * (insertCount / ln(k) + extractCount * k / ln(k))
    // choosing natural k
    // which is equal to choosing best of [t] and [t+1], where t is such as:
    // -> t > 0, f(t) = t * (ln(t) - 1) == insertCount / extractCount
    // easy to see f(t) is increasing for t > 0, f(e) == 0
    // so we will use binary search on [e, e^2 + a / b] as f(e) == 0 and f(e^2 + a / b) >= a / b
    // (here a = insertCount, b = extractCount)
    double y = (double)insertCount / extractCount;
    double l = exp(1), r = exp(2) + y;
    int cnt_iters = 5 + (int)ceil(log2(r - l));
    for (int i = 0; i < cnt_iters; ++i) {
        double m = (l + r) / 2;
        if (func_support(m) < y) {
            l = m;
        }
        else {
            r = m;
        }
    }
    // now needed t is in [l, r] and abs(r - l) < 1
    // so to optimize over [t], [t + 1] we can optimize over [l], ... [r] + 1
    int best_x = (int)floor(l);
    for (int x = (int)floor(l) + 1; x <= floor(r) + 1; ++x) {
        if (func_optimized(x, insertCount, extractCount) < func_optimized(best_x, insertCount, extractCount)) {
            best_x = x;
        }
    }

    k = best_x;
}


template <class Key>
void Heap<Key>::merge(Heap<Key> &otherHeap) {
    for (int i = 0; i < otherHeap.nodes.size(); ++i) {
        insert(otherHeap.nodes[i]->key);
    }
    otherHeap.nodes.clear();
}



template<class Key>
Heap<Key>::Node::Node(Key key_, size_t index_) {
    key = key_;
    index = index_;
}


template <class Key>
void Heap<Key>::swap_nodes(size_t i, size_t j) {
    nodes[i]->index = j;
    nodes[j]->index = i;
    swap(nodes[i], nodes[j]);
}


template <class Key>
void Heap<Key>::siftUp(size_t index) {
    while (index > 0 && nodes[index]->key < nodes[(index - 1) / k]->key) {
        swap_nodes(index, (index - 1) / k);
        index = (index - 1) / k;
    }
}


template<class Key>
void Heap<Key>::siftDown(size_t index) {
    while (index * k + 1 < nodes.size()) {
        size_t min_id = index * k + 1;
        for (int i = 2; i <= k; ++i) {
            if (index * k + i < nodes.size() && nodes[index * k + i]->key < nodes[min_id]->key) {
                min_id = index * k + i;
            }
        }

        if (nodes[min_id]->key < nodes[index]->key) {
            swap_nodes(min_id, index);
            index = min_id;
        }
        else {
            break;
        }
    }
}


template <class Key>
double Heap<Key>::func_optimized(double x, int a, int b) {
    return a / log(x) + b * x / log(x);
}


template<class Key>
double Heap<Key>::func_support(double x) {
    return x * (log(x) - 1);
}



#endif //HEAP_HEAP_H
