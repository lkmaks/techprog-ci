#ifndef HEAP_FIBONACCIHEAP_H
#define HEAP_FIBONACCIHEAP_H


#include "Vector.h"
#include <cstdlib>


template <class Key>
class FibonacciHeap {
private:
    class Node;
public:
    class Pointer {
        friend FibonacciHeap<Key>;
    private:
        Node *ptr;
        explicit Pointer(Node *ptr_);
    public:
        Pointer();
        Key getKey();
    };

    FibonacciHeap();

    bool is_empty() const;
    Pointer insert(Key);
    Key get_min() const;
    Key extract_min();
    void merge(FibonacciHeap&);
    void decrease(Pointer, Key);

private:
    class Node {
        friend FibonacciHeap<Key>;
    private:
        Key key;
        Node *parent, *child, *prev, *next;
        size_t degree;
        bool mark;
        explicit Node(Key);
    };

    Node *min_node;

    void attach(Node*, Node*);
    void add_node_to_roots(Node*);
    void consolidate(Node*);
    void cut(Node*);
    void cascading_cut(Node*);
};



template <class Key>
FibonacciHeap<Key>::Pointer::Pointer(Node *ptr_) {
    ptr = ptr_;
}


template <class Key>
FibonacciHeap<Key>::Pointer::Pointer() {
    ptr = nullptr;
}


template <class Key>
Key FibonacciHeap<Key>::Pointer::getKey() {
    return ptr->key;
}



template <class Key>
FibonacciHeap<Key>::FibonacciHeap() {
    min_node = nullptr;
}


template <class Key>
bool FibonacciHeap<Key>::is_empty() const {
    return min_node == nullptr;
}


template<class Key>
typename FibonacciHeap<Key>::Pointer FibonacciHeap<Key>::insert(Key key) {
    Node *new_node = new Node(key);
    add_node_to_roots(new_node);
    return Pointer(new_node);
}


template<class Key>
Key FibonacciHeap<Key>::get_min() const {
    if (is_empty()) {
        throw std::logic_error("FibonacciHeap instance is empty");
    }
    return min_node->key;
}


template <class Key>
Key FibonacciHeap<Key>::extract_min() {
    if (is_empty()) {
        throw std::logic_error("FibonacciHeap instance is empty");
    }
    Key ret = min_node->key;

    Node *cur = min_node->child;
    if (cur != nullptr) {
        Vector<Node*> children;
        children.push_back(cur);
        Node *start = cur;
        cur = cur->next;
        while (cur != start) {
            children.push_back(cur);
            cur = cur->next;
        }
        for (int i = 0; i < children.size(); ++i) {
            Node *cur = children[i];
            cur->parent = nullptr;
            add_node_to_roots(cur);
        }
    }

    Node *prev_node = min_node->prev;
    Node *next_node = min_node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    bool flag = (next_node == min_node);

    delete min_node;

    min_node = nullptr;
    if (!flag) {
        consolidate(next_node);
    }

    return ret;
}


template<class Key>
void FibonacciHeap<Key>::merge(FibonacciHeap &otherHeap) {
    if (min_node == nullptr) {
        min_node = otherHeap.min_node;
    }
    else if (otherHeap.min_node == nullptr) {
        return;
    }
    else if (min_node->next == min_node) {
        otherHeap.add_node_to_roots(min_node);
        min_node = otherHeap.min_node;
    }
    else if (otherHeap.min_node->next == otherHeap.min_node) {
        add_node_to_roots(otherHeap.min_node);
    }
    else {
        Node *a = min_node, *b = min_node->next;
        Node *c = otherHeap.min_node, *d = otherHeap.min_node->prev;
        a->next = c;
        c->prev = a;
        d->next = b;
        b->prev = a;
        if (otherHeap.min_node->key < min_node->key) {
            min_node = otherHeap.min_node;
        }
    }
    otherHeap.min_node = nullptr;
}


template<class Key>
void FibonacciHeap<Key>::decrease(Pointer ptr, Key key) {
    Node *cur = ptr.ptr;
    if (cur->key < key) {
        throw std::invalid_argument("Decrease new value is bigger than current value");
    }

    cur->key = key;
    Node *par = cur->parent;
    if (par != nullptr && cur->key < par->key) {
        cut(cur);
        cascading_cut(par);
    }
    else if (cur->key < min_node->key) {
        min_node = cur;
    }
}



template<class Key>
FibonacciHeap<Key>::Node::Node(Key key_) {
    key = key_;
    parent = child = prev = next = nullptr;
    degree = 0;
    mark = false;
}


template <class Key>
void FibonacciHeap<Key>::attach(Node *root, Node *child) {
    // attaches child node to root node

    Node *root_child = root->child;
    if (root_child == nullptr) {
        root->child = child;
        child->next = child->prev = child;
    }
    else {
        Node *next_root_child = root_child->next;
        root_child->next = child, next_root_child->prev = child;
        child->prev = root_child, child->next = next_root_child;
    }
    ++root->degree;
    child->parent = root;
}


template <class Key>
void FibonacciHeap<Key>::add_node_to_roots(Node *node) {
    // assume node is a root in a tree (parent == null)
    // and we add it to roots list maintaining min_node

    if (min_node == nullptr) {
        min_node = node;
        node->next = node->prev = node;
    }
    else {
        Node *next_node = min_node->next;
        node->next = next_node, node->prev = min_node;
        min_node->next = node, next_node->prev = node;
        if (node->key < min_node->key) {
            min_node = node;
        }
    }
}


template <class Key>
void FibonacciHeap<Key>::consolidate(Node *root_node) {
    // param root_node - arbitrary node in roots list
    Vector<Node*> arr;
    Node *cur = root_node;
    Node *start = cur;
    arr.push_back(cur);
    cur = cur->next;
    while (cur != start ) {
        arr.push_back(cur);
        cur = cur->next;
    }

    size_t max_degree = 0;
    for (int i = 0; i < arr.size(); ++i) {
        arr[i]->prev = arr[i]->next = nullptr;
        max_degree = max(max_degree, arr[i]->degree);
    }

    Vector<Node*> con;
    for (int i = 0; i < arr.size(); ++i) {
        cur = arr[i];
        while (con.size() <= cur->degree) {
            con.push_back(nullptr);
        }
        while (con[cur->degree] != nullptr) {
            Node *cur2 = con[cur->degree];
            if (cur->key > cur2->key) {
                swap(cur, cur2);
            }
            con[cur->degree] = nullptr;
            attach(cur, cur2);
            if (con.size() <= cur->degree) {
                con.push_back(nullptr);
            }
        }
        con[cur->degree] = cur;
    }
    for (int i = 0; i < con.size(); ++i) {
        if (con[i] != nullptr) {
            add_node_to_roots(con[i]);
        }
    }
}


template<class Key>
void FibonacciHeap<Key>::cut(Node *node) {
    Node *par = node->parent;
    --par->degree;
    par->child = nullptr;

    Node *prev_node = node->prev, *next_node = node->next;
    prev_node->next = next_node, next_node->prev = prev_node;
    if (next_node != node) {
        par->child = next_node;
    }

    node->prev = node->next = node;
    node->parent = nullptr;
    node->mark = false;
    add_node_to_roots(node);
}

template<class Key>
void FibonacciHeap<Key>::cascading_cut(Node *node) {
    Node *par = node->parent;
    if (par != nullptr) {
        if (!node->mark) {
            node->mark = true;
        }
        else {
            cut(node);
            cascading_cut(par);
        }
    }
}


#endif //HEAP_FIBONACCIHEAP_H
