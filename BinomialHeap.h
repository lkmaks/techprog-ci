#ifndef HEAP_BINOMIALHEAP_H
#define HEAP_BINOMIALHEAP_H


#include "Vector.h"


template <class Key>
class BinomialHeap {
private:
    class Node;
    class LayerNode;

public:
    class Pointer {
        friend BinomialHeap<Key>;
    private:
        Node *ptr;
        explicit Pointer(Node *ptr_);
    public:
        Pointer();
        Key getKey();
    };

    bool is_empty() const;
    Pointer insert(Key);
    Key get_min() const;
    Key extract_min();
    void merge(BinomialHeap &otherHeap);
    void erase(Pointer ptr);
    void change(Pointer, Key);

private:
    class Node {
        friend BinomialHeap<Key>;
    public:
        Node *first_child, *next_brother, *prev_brother;
        LayerNode *brother_layer_node, *child_layer_node;
        size_t order;
        Key key;

        explicit Node(Key);
    };

    // LayerNode instance stores parent for all nodes with the same parent
    // Each node has a LayerNode for its children (even if there is no children yet)
    class LayerNode {
        friend BinomialHeap<Key>;
        friend Node;
    private:
        Node *parent;
        explicit LayerNode(Node*);
    };

    Vector<Node*> roots;
    Node *min_node;

    Node *get_parent(Node*);
    void swap_with_parent(Node*);
    Node *merge_binomial_trees(Node *a, Node *b);
    void attach(Node *root, Node *child);
    void add_nodes(Vector<Node*>&, Vector<Node*>&);
    void update_min_node_and_roots();
};



template <class Key>
BinomialHeap<Key>::Pointer::Pointer() {
    ptr = nullptr;
}


template <class Key>
BinomialHeap<Key>::Pointer::Pointer(Node *ptr_) {
    ptr = ptr_;
}


template <class Key>
Key BinomialHeap<Key>::Pointer::getKey() {
    return ptr->key;
}



template <class Key>
bool BinomialHeap<Key>::is_empty() const {
    return roots.is_empty();
}


template <class Key>
Key BinomialHeap<Key>::get_min() const {
    if (is_empty()) {
        throw std::logic_error("BinomialHeap instance is empty");
    }
    return min_node->key;
}


template <class Key>
void BinomialHeap<Key>::merge(BinomialHeap &otherHeap) {
    add_nodes(roots, otherHeap.roots);
    otherHeap.roots.clear();
    update_min_node_and_roots();
}


template <class Key>
typename BinomialHeap<Key>::Pointer BinomialHeap<Key>::insert(Key key) {
    Node *cur_tree = new Node(key);
    Node *ptr_to_element = cur_tree;
    size_t i = 0;
    while (i < roots.size() && roots[i] != nullptr) {
        cur_tree = merge_binomial_trees(cur_tree, roots[i]);
        roots[i] = nullptr;
        ++i;
    }
    if (i == roots.size()) {
        roots.push_back(cur_tree);
    }
    else {
        roots[i] = cur_tree;
    }

    update_min_node_and_roots();

    return Pointer(ptr_to_element);
}


template <class Key>
void BinomialHeap<Key>::erase(Pointer ptr) {
    if (is_empty()) {
        throw std::logic_error("BinomialHeap instance is empty");
    }

    Node *cur = ptr.ptr;
    while (get_parent(cur) != nullptr) {
        swap_with_parent(cur);
    }

    Vector<Node*> children;
    Node *cur_child = cur->first_child;
    while (cur_child != nullptr) {
        children.push_back(cur_child);
        cur_child = cur_child->next_brother;
    }
    for (int i = 0; i < children.size(); ++i) {
        children[i]->brother_layer_node = nullptr;
        children[i]->prev_brother = nullptr;
        children[i]->next_brother = nullptr;
    }
    children.reverse();

    roots[cur->order] = nullptr;
    delete cur;

    add_nodes(roots, children);

    update_min_node_and_roots();
}


template <class Key>
Key BinomialHeap<Key>::extract_min() {
    if (is_empty()) {
        throw std::logic_error("BinomialHeap instance is empty");
    }
    Key res = min_node->key;
    erase(Pointer(min_node));
    return res;
}


template <class Key>
void BinomialHeap<Key>::change(Pointer ptr, Key key) {
    erase(ptr);
    insert(key);
}



template <class Key>
BinomialHeap<Key>::LayerNode::LayerNode(Node *node) {
    parent = node;
}


template <class Key>
BinomialHeap<Key>::Node::Node(Key key_) {
    order = 0;
    first_child = next_brother = prev_brother = nullptr;
    key = key_;
    child_layer_node = new LayerNode(this);
    brother_layer_node = nullptr;
}


template <class Key>
typename BinomialHeap<Key>::Node *BinomialHeap<Key>::get_parent(Node *node) {
    if (node->brother_layer_node == nullptr) {
        return nullptr;
    }
    else {
        return node->brother_layer_node->parent;
    }
}


template <class Key>
void BinomialHeap<Key>::attach(Node *root, Node *child) {
    child->next_brother = root->first_child;
    if (root->first_child != nullptr) {
        root->first_child->prev_brother = child;
    }
    child->brother_layer_node = root->child_layer_node;
    root->first_child = child;
    ++root->order;
}


template <class Key>
typename BinomialHeap<Key>::Node* BinomialHeap<Key>::merge_binomial_trees(Node *a, Node *b) {
    if (a == nullptr) {
        return b;
    }
    else if (b == nullptr) {
        return a;
    }
    else if (a->key < b->key) {
        attach(a, b);
        return a;
    }
    else {
        attach(b, a);
        return b;
    }
}


template <class Key>
void BinomialHeap<Key>::add_nodes(Vector<Node*> &dest, Vector<Node*> &source) {
    // assume that in dest and source trees are in order-increasing order

    while (dest.size() < source.size()) {
        dest.push_back(nullptr);
    }
    while (source.size() < dest.size()) {
        source.push_back(nullptr);
    }

    Node *carry = nullptr;
    for (int i = 0; i < dest.size(); ++i) {
        if (carry == nullptr) {
            Node *sum = merge_binomial_trees(dest[i], source[i]);
            if (sum == nullptr || sum->order == i) {
                dest[i] = sum;
            }
            else {
                carry = sum;
                dest[i] = nullptr;
            }
        }
        else {
            if (dest[i] == nullptr || source[i] == nullptr) {
                Node *sum = merge_binomial_trees(dest[i], merge_binomial_trees(source[i], carry));
                if (sum->order == i) {
                    dest[i] = sum;
                    carry = nullptr;
                }
                else {
                    carry = sum;
                    dest[i] = nullptr;
                }
            }
            else {
                Node *sum = merge_binomial_trees(dest[i], source[i]);
                dest[i] = carry;
                carry = sum;
            }
        }
    }
    if (carry != nullptr) {
        dest.push_back(carry);
    }
}


template<typename Key>
void BinomialHeap<Key>::update_min_node_and_roots() {
    min_node = nullptr;
    for (int i = 0; i < roots.size(); ++i) {
        if (min_node == nullptr && roots[i] != nullptr) {
            min_node = roots[i];
        }
        else if (min_node != nullptr && roots[i] != nullptr && roots[i]->key < min_node->key) {
            min_node = roots[i];
        }
    }
    while (!roots.is_empty() && roots[roots.size() - 1] == nullptr) {
        roots.pop_back();
    }
}


template <class Key>
void BinomialHeap<Key>::swap_with_parent(Node *child) {
    Node *par = get_parent(child);

    Node *par_prev = par->prev_brother, *par_next = par->next_brother;
    Node *child_prev = child->prev_brother, *child_next = child->next_brother;
    LayerNode *child_layer = child->brother_layer_node;
    LayerNode *child_child_layer = child->child_layer_node;


    if (get_parent(par) != nullptr && get_parent(par)->first_child == par) {
        get_parent(par)->first_child = child;
    }


    child->prev_brother = par_prev, child->next_brother = par_next;
    par->prev_brother = child_prev, par->next_brother = child_next;
    if (par_prev != nullptr) {
        par_prev->next_brother = child;
    }
    if (par_next != nullptr) {
        par_next->prev_brother = child;
    }
    if (child_prev != nullptr) {
        child_prev->next_brother = par;
    }
    if (child_next != nullptr) {
        child_next->prev_brother = par;
    }


    if (child == par->first_child) {
        par->first_child = child->first_child;
        child->first_child = par;
    }
    else {
        swap(child->first_child, par->first_child);
    }

    swap(child->order, par->order);

    swap(child->brother_layer_node, par->brother_layer_node);

    par->child_layer_node = child->child_layer_node;
    child->child_layer_node = child_layer;

    child_layer->parent = child;
    child_child_layer->parent = par;
}


#endif //HEAP_BINOMIALHEAP_H
