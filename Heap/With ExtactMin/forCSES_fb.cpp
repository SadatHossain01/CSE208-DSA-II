#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Edge {
    int u, v;
    ll w;
};

struct Pair {
    int u;
    ll w;
    bool operator<(const Pair& other) const { return w < other.w; }
    bool operator<=(const Pair& other) const { return w <= other.w; }
    bool operator>(const Pair& other) const { return w > other.w; }
    bool operator>=(const Pair& other) const { return w >= other.w; }
    bool operator==(const Pair& other) const {
        return u == other.u && w == other.w;
    }
    Pair operator-(int dx) const {
        Pair ret = {u, w - dx};
        return ret;
    }
    Pair& operator=(const Pair& other) {
        u = other.u;
        w = other.w;
        return *this;
    }
};

void __print(int x) { cerr << x; }
void __print(long long x) { cerr << x; }
void _print() { cerr << "]\n"; }
void __print(const string& x) { cerr << '\"' << x << '\"'; }
template <typename T, typename... V>
void _print(T t, V... v) {
    __print(t);
    if (sizeof...(v)) cerr << ", ";
    _print(v...);
}
void __print(const Pair& x) {
    cerr << '\"' << "u: " << x.u << ", w: " << x.w << '\"';
}
#ifdef LOCAL
#define debug(x...)               \
    cerr << "[" << #x << "] = ["; \
    _print(x)
#else
#define debug(x...)
#endif

const ll INF = 2e16;
int n_vertices, n_edges;
vector<vector<Edge>> adj;
vector<ll> dist_fb;
vector<int> len_fb;
vector<bool> visited;

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

// Min Heap
template <typename T>
class FibHeap {
   private:
    Node<T>* min;
    Node<T>** mp;
    int n, maxSize;

    void printNode(Node<T>* cur) {
#ifndef LOCAL
        return;
#endif
        if (cur == nullptr) {
            debug("null pointer");
        } else {
            debug(cur->left->val, cur->val, cur->right->val, cur->degree);
        }
    }

    void printRootList(Node<T>* start) {
#ifndef LOCAL
        return;
#endif
        Node<T>* cur = start;
        debug("Printing Started");
        debug(min->val, n);
        while (true) {
            debug(cur->left->val, cur->val, cur->right->val, cur->degree);
            cur = cur->right;
            if (cur == start) break;
        }
        debug("Printing Done");
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
            a->right->left = b->left;
            b->left->right = a->right;
            a->right = b;
            b->left = a;
        }
    }

    void remove(Node<T>* x) {
        Node<T>* prev = x->left;
        Node<T>* next = x->right;
        prev->right = next;
        next->left = prev;
    }

    void link(Node<T>* y, Node<T>* x) {
        // debug("Making ", y->val, " a child of ", x->val);
        remove(y);
        // make y a child of x
        y->parent = x;
        y->left = y->right = y;
        Node<T>* prevChild = x->child;
        if (prevChild == nullptr) {
            x->child = y;
        } else {
            // printNode(prevChild);
            concatenate(prevChild, y);
            // printNode(prevChild);
        }
        // increment degree[x], clear mark[y]
        x->degree++;
        y->marked = false;
    }

    void consolidate() {
        debug("Consolidation Start");
        const int lg = __lg(n) + 2;
        Node<T>** A = new Node<T>*[lg];
        for (int i = 0; i < lg; i++) A[i] = nullptr;
        // iterate over all nodes w in the root list

        // fix the min first
        Node<T>* w = min;
        Node<T>* start = w;
        // debug("Consolidation while 1 Start");
        while (true) {
            if (w->val < min->val) min = w;
            w = w->right;
            if (w == start) break;
        }
        // debug("Consolidation while 1 End");

        // debug("Consolidation while 2 Start");
        w = min;
        start = w;
        while (true) {
            Node<T>* x = w;
            // debug(x->left->val, x->val, x->right->val, start->val);
            int d = x->degree;
            if (A[d] != nullptr && A[d] == x) break;
            while (A[d] != nullptr) {
                Node<T>* y = A[d];  // another node with same degree as x
                if (y->val < x->val) swap(x, y);
                if (start == y) start = x;
                if (w == y) w = x;
                if (min == y) min = x;  // where were you all day?
                link(y, x);  // remove y from rootlist, make y a child of x,
                             // degree[x] is incremented, mark[y] is cleared
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            w = w->right;
            if (w == start) break;
        }
        // debug("Consolidation while 2 End");

        for (int i = 0; i < lg; i++) {
            if (A[i] == nullptr) continue;
            if (min == nullptr)
                min = A[i];
            else if (A[i]->val < min->val)
                min = A[i];
        }

        // assert(min != nullptr);
        delete[] A;
        debug("Consolidation End");
        // printRootList(min);
    }

   public:
    FibHeap(int size) {
        n = 0;
        min = nullptr;
        maxSize = size;
        mp = new Node<T>*[maxSize + 10];
        for (int i = 0; i < maxSize + 10; i++) mp[i] = nullptr;
    }

    ~FibHeap() {
        while (min != nullptr) {
            if (min == min->right) {
                delete min;
                min = nullptr;
            } else
                remove(min->right);
        }
        n = 0;
        for (int i = 0; i < maxSize + 10; i++) {
            if (mp[i] != nullptr) delete mp[i];
        }
        delete[] mp;
    }

    void insert(T val) {
        Node<T>* x = new Node<T>(val);
        x->left = x->right = x;
        // assert(val.u < maxSize + 10);
        mp[val.u] = x;
        // concatenating the root list H with the root list containing x
        // and updating the min
        if (min == nullptr) {
            min = x;
        } else {
            concatenate(min, x);
            if (x->val < min->val) min = x;
        }
        n++;
        // debug("insertion", val, n);
        // printRootList(min);
    }

    T getMin() const {
        assert(n > 0 && min != nullptr);
        return min->val;
    }

    int getSize() const { return n; }

    bool isEmpty() const { return n == 0; }

    T extractMin() {
        assert(n > 0 && min != nullptr);
        debug("before extraction", min->left->val, min->val, min->right->val,
              n);

        Node<T>* z = min;

        // for each child x of z, add x to the root list
        // or just concatenate the child list of z to the root list, both works

        // doing all at once
        if (z->child != nullptr) {
            Node<T>* cur = z->child;
            while (true) {
                cur->parent = nullptr;
                z->degree--;
                cur = cur->right;
                if (cur == z->child) break;
            }
            concatenate(min, z->child);
        }

        // the book-prescribed version,
        // that is promoting the children one by one
        // if (z->child != nullptr) {
        //     debug("Working with child of", z->val, z->degree, z->child->val);
        //     Node<T>* cur = z->child;
        //     while (true) {
        //         debug(cur->left->val, cur->val, cur->right->val,
        //         z->child->val); Node<T>* upNext = cur->right; cur->left =
        //         cur->right = cur; cur->parent = nullptr; z->degree--;
        //         concatenate(min, cur);
        //         cur = upNext;
        //         if (cur == z->child) break;
        //     }
        // }

        // remove z from the root list
        remove(z);
        if (z == z->right) {  // only one member in the root list
            min = nullptr;
        } else {
            min = z->right;
            consolidate();
        }
        n--;
        debug("Extraction done", z->val);
        if (min != nullptr) {
            debug("After Extraction", min->left->val, min->val, min->right->val,
                  n);
        }
        T ret = z->val;
        delete z;
        mp[ret.u] = nullptr;
        return ret;
    }

    void cut(Node<T>* x, Node<T>* y) {
        // remove x from the child list of y
        if (x->right == x)
            y->child = nullptr;
        else {
            y->child = x->right;  // what is this behaviour bro?
            remove(x);
        }
        y->degree--;
        // add x to the root list
        x->left = x->right = x;
        concatenate(min, x);
        x->parent = nullptr;
        x->marked = false;
    }

    void cascadingCut(Node<T>* y) {
        // debug("Cascading Cut Start", y->val);
        Node<T>* z = y->parent;
        if (z != nullptr) {
            if (y->marked == false)
                y->marked = true;
            else {
                cut(y, z);
                cascadingCut(z);
            }
        }
        debug("Cascading Cut End", y->val);
    }

    void decreaseKey(const T& p, long long newVal) {
        Node<T>* x = mp[p.u];
        assert(x != nullptr);
        assert(newVal <= x->val.w);
        x->val.w = newVal;
        Node<T>* y = x->parent;
        if (y != nullptr && x->val < y->val) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->val < min->val) min = x;
    }
};

void dijkstra_fb(int s) {
    // initialize-single-source-distance
    fill(dist_fb.begin(), dist_fb.end(), INF);
    fill(len_fb.begin(), len_fb.end(), INF);
    dist_fb[s] = 0;
    len_fb[s] = 0;
    FibHeap<Pair> fq(n_vertices);
    // 0-based
    for (int i = 0; i < n_vertices; i++) {
        Pair p;
        if (i == s)
            p = {i, 0};
        else
            p = {i, INF};
        fq.insert(p);
    }
    while (!fq.isEmpty()) {
        // extract-min
        Pair cur = fq.extractMin();
        // debug(cur);
        int u = cur.u;
        for (const Edge& e : adj[u]) {
            int v = e.v;
            // relaxation
            if (dist_fb[v] > dist_fb[u] + e.w) {
                fq.decreaseKey({v, dist_fb[v]}, dist_fb[u] + e.w);
                dist_fb[v] = dist_fb[u] + e.w;
                len_fb[v] = len_fb[u] + 1;
            }
        }
    }
    cerr << "Fibonacci done\n";
}

int main() {
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    cin >> n_vertices >> n_edges;

    adj.resize(n_vertices + 2);
    dist_fb.resize(n_vertices + 2);
    len_fb.resize(n_vertices + 2);
    visited.resize(n_vertices + 2);

    for (int i = 0; i < n_edges; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        Edge e = {a, b, c};
        adj[a].push_back(e);
    }

    dijkstra_fb(0);

    for (int i = 0; i < n_vertices; i++) {
        cout << dist_fb[i] << " ";
    }
}