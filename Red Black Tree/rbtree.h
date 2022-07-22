#pragma once

#define black true
#define red false

template <typename T>
struct Node {
    bool color;  // false means red, true means black
    T key;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;

    Node() {
        color = black;  // null nodes are black
        left = right = parent = nullptr;
    }
    Node(const T& key) {
        color = red;  // any newly inserted node is black, denoted as false
        this->key = key;
        left = right = parent = nullptr;
    }
};

template <typename T>
class RBT {
   private:
    Node<T>* root;

    Node<T>* successor(Node<T>* x) {
        if (x->right != nullptr) return tree_minimum(x->right);
        Node<T>* y = x->parent;
        while (y != nullptr && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    void left_rotate(Node<T>* x) {
        Node<T>* y = x->right;
        x->right = y->left;
        y->left->parent = x;
        y->parent = x->parent;
        // now checking which child of x's parent x is, or if x has any parent at all
        if (x->parent == nullptr) root = y;  // x was the root previously
        else if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void right_rotate(Node<T>* x) {
        Node<T>* y = x->left;
        x->left = y->right;
        y->right->parent = x;
        y->parent = x->parent;
        // now checking which child of x's parent x is, or if x has any parent at all
        if (x->parent == nullptr) root = y;  // x was the root previously
        else if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
        y->right = x;
        x->parent = y;
    }

    void insert_fixup(Node<T>* z) {
        // black uncle: rotate, red uncle: color flip
        while (z->parent->color == red) {
            if (z->parent == z->parent->parent->left) {  // z is the left child
                Node<T>* y = z->parent->parent->right;   // y is z's uncle
                if (y->color == red) {                   // color flip needed
                    z->parent->color = black;            // previously red
                    y->color = black;                    // previosly red
                    z->parent->parent->color = red;
                    z = z->parent->parent;
                } else {                          // rotation needed
                    if (z == z->parent->right) {  // left rotation
                        z = z->parent;
                        left_rotate(z);
                    }
                    z->parent->color = black;
                    z->parent->parent->color = red;
                    right_rotate(z->parent->parent);
                }
            } else {                                   // z is the right child
                Node<T>* y = z->parent->parent->left;  // y is z's uncle
                if (y->color == red) {                 // color flip needed
                    z->parent->color = black;          // previously red
                    y->color = black;                  // previosly red
                    z->parent->parent->color = red;
                    z = z->parent->parent;
                } else {                         // rotation needed
                    if (z == z->parent->left) {  // right rotation
                        z = z->parent;
                        right_rotate(z);
                    }
                    z->parent->color = black;
                    z->parent->parent->color = red;
                    left_rotate(z->parent->parent);
                }
            }
        }
        root->color = black;
    }

   public:
    void insert(Node<T>* z) {
        Node<T>* y = nullptr;
        Node<T>* x = root;
        while (x != nullptr) {
            y = x;
            if (z->key < x->key) x = x->left;
            else x = x->right;
        }
        z->parent = y;
        if (y == nullptr) root = z;
        else if (z->key < y->key) y->left = z;
        else y->right = z;
        z->left = z->right = nullptr;
        z->color = red;
        // it never performs more than two rotations
        insert_fixup(z);
    }

    Node<T>* remove(Node<T>* z) {
        Node<T>* x;
        Node<T>* y;
        if (z->left == nullptr || z->right == nullptr)  // z is an external node
            y = z;
        else y = tree_successor(z);  // or tree_predecessor(z)
        if (y->left == nullptr) x = y->left;
        else x = z->right;
        x->parent = y->parent;
        if (y->parent == nullptr) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        if (y != z) z->key = y->key;
        if (y->color == black) delete_fixup(x);
        return y;
    }
};