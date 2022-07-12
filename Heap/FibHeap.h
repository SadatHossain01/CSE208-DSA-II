#pragma once
template <typename T>
struct Node {
    Node* parent;
    Node* child;
    Node* left;
    Node* right;
    T val;
    int degree;   // number of children
    bool marked;  // whether it has lost a child since becoming a child of
                  // another node
    Node(T v) : val(v) {
        parent = child = left = right = nullptr;
        degree = 0;
        marked = false;
    }
};
template <typename T>
class FibHeap {
   private:
    Node<T>* min;
    int n;

    void printRootList() {
#ifndef LOCAL
        return;
#endif
        Node<T>* cur = min;
        debug("printing started");
        debug(min->val);
        while (true) {
            debug(cur->val, cur->degree);
            cur = cur->right;
            if (cur == min) break;
        }
        debug("printing done");
    }

    void concatenate(Node<T>* a, Node<T>* b) {
        assert(a->left != nullptr && a->right != nullptr);
        assert(b->left != nullptr && b->right != nullptr);
        if (a == a->left) swap(a, b);
        if (b == b->left) {
            auto tmp = a->right;
            a->right = b;
            b->left = a;
            b->right = tmp;
            tmp->left = b;
        } else {
            a->left->right = b->right;
            b->right->left = a->left;
            a->left = b;
            b->right = a;
        }
    }

    void remove(Node<T>* x) {
        Node<T>* prev = x->left;
        Node<T>* next = x->right;
        prev->right = next;
        next->left = prev;
    }

    void link(Node<T>* y, Node<T>* x) {
        remove(y);
        // make y a child of x
        y->parent = x;
        Node<T>* prevChild = x->child;
        if (prevChild == nullptr) {
            x->child = y;
        } else
            concatenate(prevChild, y);
        // increment degree[x], clear mark[y]
        x->degree++;
        y->marked = false;
    }

    void consolidate() {
        const int lg = __lg(n) + 2;
        Node<T>** A = new Node<T>*[lg];
        for (int i = 0; i < lg; i++) A[i] = nullptr;
        // iterate over all nodes w in the root list
        Node<T>* w = this->min;
        Node<T>* start = w;
        // debug("consolidation while start");
        while (true) {
            Node<T>* x = w;
            // debug(x->val);
            int d = x->degree;
            while (A[d] != nullptr) {
                Node<T>* y = A[d];  // another node with same degree as x
                if (x->val > y->val) swap(x, y);
                if (start == y) start = x;
                if (w == y) w = x;
                link(y, x);  // remove y from rootlist, make y a child of x,
                             // degree[x] is incremented, mark[y] is cleared
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            // debug("before right shift", w->val);
            w = w->right;
            // debug("after right shift", w->val);
            if (w == start) break;
        }
        // debug("consolidation while end");
        this->min = nullptr;
        for (int i = 0; i < lg; i++) {
            if (A[i] != nullptr) {
                if (this->min != nullptr)
                    concatenate(this->min, A[i]);
                else
                    this->min = A[i];
                if (A[i]->val < this->min->val) this->min = A[i];
            }
        }
        delete[] A;
    }

    bool searchDown(T x, Node<T>* cur, Node<T>* ret) {  // useless for now
        // only goes down following the child pointers
        while (cur != nullptr) {
            if (cur->val == x) {  // found
                ret = cur;
                return true;
            } else if (cur->val >
                       x)  // child will only be higher, so will never find
                break;
            else
                cur = cur->child;
        }
        return false;  // not found
    }

   public:
    FibHeap() {
        n = 0;
        this->min = nullptr;
    }

    ~FibHeap() {
        n = 0;
        delete[] this->min;
    }

    void insert(T val) {
        Node<T>* x = new Node<T>(val);
        x->left = x->right = x;
        // concatenating the root list H with the root list containing x
        // and updating the min
        if (this->min == nullptr) {
            this->min = x;
        } else {
            concatenate(this->min, x);
            if (x->val < this->min->val) this->min = x;
        }
        n++;
        printRootList();
    }

    T getMin() const {
        assert(n > 0);
        return this->min->val;
    }

    int getSize() const { return n; }

    bool isEmpty() const { return n == 0; }

    FibHeap<T>* unite_heap(FibHeap<T>* h1, FibHeap<T>* h2) {
        FibHeap<T>* h = new FibHeap<T>();
        if (h1->min == nullptr && h2->min == nullptr) return h;
        if (h1->min == nullptr) swap(h1, h2);
        h->min = h1->min;
        if (h2->min == nullptr) {
            h->n = h1->n;
            delete h1;
        } else {
            concatenate(h->min, h2->min);
            h->n = h1->n + h2->n;
            if (h2->min->val < h1->min) h->min = h2->min;
            delete h1;
            delete h2;
        }
        return h;
    }

    T extractMin() {
        assert(n > 0);
        debug(min->val, min->right->val);
        Node<T>* z = this->min;
        // for each child x of z, add x to the root list
        // or just concatenate the child list of z to the root list
        // first update their parents to nullptr though
        if (z->child != nullptr) {
            Node<T>* cur = z->child;
            while (true) {
                debug(cur->val, cur->right->val, z->child->val);
                cur->parent = nullptr;
                z->degree--;
                cur = cur->right;
                if (cur == z->child) break;
            }
            concatenate(this->min, z->child);
        }
        // remove z from the root list
        remove(z);
        if (z == z->right)  // only one member in the root list
            this->min = nullptr;
        else {
            this->min = z->right;
            // debug("consolidation begin");
            consolidate();
            // debug("consolidation end");
        }
        n--;
        return z->val;
    }
};