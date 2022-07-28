#pragma once

#define black true
#define red false

#include <cstdlib>

#include "header.h"

template <typename T>
struct Node {
    bool color;  // false for red, true for black
    int size;    // size of the subtree rooted at this node
    T key;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;

    Node() {
        color = red;  // any newly inserted node is red
        left = right = parent = nullptr;
        size = 1;
    }

    Node(const T& key) {
        color = red;  // any newly inserted node is red
        this->key = key;
        left = right = parent = nullptr;
        size = 1;
    }

    void print(bool isNIL = false) {
#ifndef LOCAL
        return;
#endif
        string c = (color ? "BLACK" : "RED");
        string b = (isNIL ? "NIL" : "NO");
        debug(c, size, key, b);
    }
};

template <typename T>
class RBT {
   private:
    Node<T>* root;
    Node<T>* NIL;

    Node<T>* tree_minimum(Node<T>* x) {
        while (x->left != NIL) x = x->left;
        return x;
    }

    Node<T>* tree_maximum(Node<T>* x) {
        while (x->right != NIL) x = x->right;
        return x;
    }

    Node<T>* tree_predecessor(Node<T>* x) {
        if (x->left != NIL) return tree_maximum(x->left);
        Node<T>* y = x->parent;
        while (y != NIL && x == y->left) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node<T>* tree_successor(Node<T>* x) {
        if (x->right != NIL) return tree_minimum(x->right);
        Node<T>* y = x->parent;
        while (y != NIL && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node<T>* search_help(Node<T>* x, const T& k) {
        while (x != NIL && k != x->key) {
            if (k < x->key) x = x->left;
            else x = x->right;
        }
        return x;
    }

    void left_rotate(Node<T>* x) {
        Node<T>* y = x->right;

        x->right = y->left;  // turn y's left subtree into x's right subtree
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        // now checking which child of x's parent x is, or if x has any parent at all
        if (x->parent == NIL) root = y;  // x was the root previously
        else if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;

        // fixing the subtree sizes
        x->size = x->left->size + x->right->size + 1;
        y->size = y->left->size + y->right->size + 1;
    }

    void right_rotate(Node<T>* x) {
        Node<T>* y = x->left;

        x->left = y->right;  // turn y's right subtree into x's left subtree
        if (y->right != NIL) y->right->parent = x;
        y->parent = x->parent;
        // now checking which child of x's parent x is, or if x has any parent at all
        if (x->parent == NIL) root = y;  // x was the root previously
        else if (x->parent->left == x) x->parent->left = y;
        else x->parent->right = y;
        y->right = x;
        x->parent = y;

        // fixing the subtree sizes
        x->size = x->left->size + x->right->size + 1;
        y->size = y->left->size + y->right->size + 1;
    }

    void insert_fixup(Node<T>* z) {
        // black uncle: rotate, red uncle: color flip
        while (z->parent->color == red) {                // a red node's both children must be black
            if (z->parent == z->parent->parent->left) {  // z.parent is a left child
                Node<T>* y = z->parent->parent->right;   // y is z's uncle
                if (y->color == red) {                   // red uncle, so color flip needed
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
            } else {                                   // z.parent is a right child
                Node<T>* y = z->parent->parent->left;  // y is z's uncle
                if (y->color == red) {                 // red uncle, so color flip needed
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
        debug("insert fixup done");
    }

    void insert_help(Node<T>* z) {
        Node<T>* y = NIL;
        Node<T>* x = root;
        while (x != NIL) {
            debug("insert loop");
            y = x;
            if (z->key < x->key) x = x->left;
            else x = x->right;
        }
        z->parent = y;
        if (y == NIL) root = z;
        else if (z->key < y->key) y->left = z;
        else y->right = z;
        z->left = z->right = NIL;
        z->color = red;

        Node<T>* cur = z->parent;
        while (cur != NIL) {
            cur->size++;
            cur = cur->parent;
        }
        // it never performs more than two rotations
        insert_fixup(z);
    }

    void transplant(Node<T>* x, Node<T>* y) {
        if (x->parent == NIL) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->parent = x->parent;
    }

    void remove_fixup(Node<T>* x) {
        while (x->color == black && x != root) {
            if (x == x->parent->left) {
                Node<T>* w = x->parent->right;  // set w as x's sibling
                if (w->color == red) {
                    w->color = black;
                    x->parent->color = red;
                    left_rotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == black && w->right->color == black) {
                    w->color = red;
                    x = x->parent;
                } else {
                    if (w->right->color == black) {
                        w->left->color = black;
                        w->color = red;
                        right_rotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = black;
                    w->right->color = black;
                    left_rotate(x->parent);
                    x = root;
                }
            } else {
                Node<T>* w = x->parent->left;  // set w as x's sibling
                if (w->color == red) {
                    w->color = black;
                    x->parent->color = red;
                    right_rotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == black && w->left->color == black) {
                    w->color = red;
                    x = x->parent;
                } else {
                    if (w->left->color == black) {
                        w->right->color = black;
                        w->color = red;
                        left_rotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = black;
                    w->left->color = black;
                    right_rotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = black;
        debug("remove fixup done");
    }

    void remove_help(Node<T>* z) {
        Node<T>* y = z;
        Node<T>* x = nullptr;
        bool y_original_color = y->color;
        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = tree_minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            y->size = z->size;
        }
        root->print();
        Node<T>* cur = x->parent;
        while (cur != NIL) {
            debug("delete loop");
            cur->print();
            cur->size--;
            cur = cur->parent;
        }
        if (y_original_color == black) remove_fixup(x);
    }

    int lower_count_help(Node<T>* x, const T& k) {
        x->print(x == NIL);
        if (x == NIL) return 0;
        else if (x->key == k) return x->left->size;
        else if (x->key < k) return 1 + x->left->size + lower_count_help(x->right, k);
        else return lower_count_help(x->left, k);
    }

    void deallocate(Node<T>* x) {
        if (x == NIL) return;
        deallocate(x->left);
        deallocate(x->right);
        delete x;
    }

   public:
    RBT() {
        NIL = new Node<T>();
        NIL->color = black;
        NIL->size = 0;
        root = NIL;
    }

    ~RBT() {
        deallocate(root);
        delete NIL;
    }

    bool search(const T& k) {
        Node<T>* ret = search_help(root, k);
        if (ret == NIL) return false;
        else return true;
    }

    bool insert(const T& k) {
        bool present = search(k);
        if (present) return false;  // key already present
        else {
            Node<T>* another = new Node<T>(k);
            insert_help(another);
            return true;  // insert successful
        }
    }

    bool remove(const T& k) {
        Node<T>* ret = search_help(root, k);
        if (ret == NIL) return false;  // key not present
        else {
            remove_help(ret);
            delete ret;
            return true;  // delete successful
        }
    }

    int lower_count(const T& k) { return lower_count_help(root, k); }
};