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

    void concatenate(Node<T>* a, Node<T>* b) {
        auto tmp = a->right;
        a->right = b;
        b->left = a;
        b->right = tmp;
        tmp->left = b;
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
        if (prevChild == nullptr)
            x->child = y;
        else {
            x->child = y;
            y->right = prevChild;
            prevChild->left = y;
        }
        // increment degree[x], clear mark[y]
        x->degree++;
        y->marked = false;
    }

    void consolidate() {
        const int lg = __lg(this->n + 2);
        Node<T>** A = new Node<T>*[lg];
        for (int i = 0; i < lg; i++) A[i] = nullptr;
        // iterate over all nodes w in the root list
        Node<T>* w = this->min;
        Node<T>* start = w;
        while (true) {
            Node<T>* x = w;
            int d = x->degree;
            while (A[d] != nullptr) {
                Node<T>* y = A[d];  // same degree as x
                if (x->val > y->val) {
                    swap(x, y);
                }
                link(y, x);  // remove y from rootlist, make y a child of x,
                             // degree[x] is incremented, mark[y] is cleared
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            w = w->right;
            if (w == start) break;
        }
        this->min = nullptr;
        for (int i = 0; i < lg; i++) {
            if (A[i] != nullptr) {
                concatenate(this->min, A[i]);
                if (this->min == nullptr || A[i]->val < this->min->val)
                    this->min = A[i];
            }
        }
        delete[] A;
    }
    bool searchDown(T x, Node<T>* cur, Node<T>* ret) {
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
        // concatenating the root list H with the root list containing x
        // and updating the min
        if (this->min == nullptr) {
            this->min = x;
            this->min->left = this->min->right = x;
        } else {
            concatenate(this->min, x);
            if (x->val < this->min->val) this->min = x;
        }
        this->n++;
    }

    T getMin() const {
        assert(this->n > 0);
        return this->min->val;
    }

    int getSize() const { return this->n; }

    bool isEmpty() const { return this->n == 0; }

    FibHeap<T>* unite_heap(FibHeap<T>* h1, FibHeap<T>* h2) {
        FibHeap<T>* h = new FibHeap<T>();
        h->min = h1->min;
        concatenate(h->min, h2->min);
        if (h1->min == nullptr || (h2->min != nullptr && h2->min < h1->min))
            h->min = h2->min;
        h->n = h1->n + h2->n;
        delete h1;
        delete h2;
        return h;
    }

    T extractMin() {
        Node<T>* z = this->min;
        if (z == nullptr) return {-1, -1};
        // for each child x of z, add x to the root list
        Node<T>* x = z->child;
        Node<T>* start = x;
        while (true) {
            if (x == nullptr) break;
            concatenate(this->min, x);
            x->parent = nullptr;
            x = x->right;
            if (x == start) break;
        }
        // remove z from the root list
        remove(z);
        if (z == z->right)  // only one member in the root list
            this->min = nullptr;
        else {
            this->min = z->right;
            consolidate();
        }
        this->n--;
        return z->val;
    }
};