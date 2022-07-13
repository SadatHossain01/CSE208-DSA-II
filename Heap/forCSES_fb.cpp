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
    bool operator<(const Pair& other) const { return w <= other.w; }
    bool operator>(const Pair& other) const { return w > other.w; }
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
void _print() { cerr << "]\n"; }
void __print(const string& x) { cerr << '\"' << x << '\"'; }
template <typename T, typename... V>
void _print(T t, V... v) {
    __print(t);
    if (sizeof...(v)) cerr << ", ";
    _print(v...);
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

template <typename T>
class FibHeap {
   private:
    Node<T>* min;
    int n;

    void printRootList(Node<T>* start) {
#ifndef LOCAL
        return;
#endif
        Node<T>* cur = start;
        debug("printing started");
        debug(min->val, n);
        while (true) {
            debug(cur->val, cur->left->val, cur->right->val, cur->degree);
            cur = cur->right;
            if (cur == start) break;
        }
        debug("printing done");
    }

    void concatenate(Node<T>* a, Node<T>* b) {
        assert(a->left != nullptr && a->right != nullptr);
        assert(b->left != nullptr && b->right != nullptr);
        if (a == a->left) swap(a, b);
        if (b == b->left) {
            // debug("single concatenation, before:");
            // debug(a->val, a->left->val, a->right->val, a->degree);
            // debug(b->val, b->left->val, b->right->val, b->degree);
            auto tmp = a->right;
            a->right = b;
            b->left = a;
            b->right = tmp;
            tmp->left = b;
            // debug("after");
            // debug(a->val, a->left->val, a->right->val, a->degree);
            // debug(b->val, b->left->val, b->right->val, b->degree);
        } else {
            // debug("double concatenation, before"");
            // debug(a->val, a->left->val, a->right->val, a->degree);
            // debug(b->val, b->left->val, b->right->val, b->degree);
            a->right->left = b->left;
            b->left->right = a->right;
            a->right = b;
            b->left = a;
            // debug("after");
            // debug(a->val, a->left->val, a->right->val, a->degree);
            // debug(b->val, b->left->val, b->right->val, b->degree);
        }
    }

    void remove(Node<T>* x) {
        Node<T>* prev = x->left;
        Node<T>* next = x->right;
        prev->right = next;
        next->left = prev;
    }

    void link(Node<T>* y, Node<T>* x) {
        // debug("making ", y->val, " child of ", x->val);
        remove(y);
        if (min == y) min = x;  // where were you all day?
        // make y a child of x
        y->parent = x;
        y->left = y->right = y;
        Node<T>* prevChild = x->child;
        if (prevChild == nullptr) {
            x->child = y;
        } else {
            // debug(prevChild->val, prevChild->left->val,
            // prevChild->right->val);
            concatenate(prevChild, y);
            // debug(prevChild->val, prevChild->left->val,
            // prevChild->right->val); debug(y->left->val, y->right->val);
        }
        // increment degree[x], clear mark[y]
        x->degree++;
        y->marked = false;
    }

    void consolidate() {
        debug("consolidation start");
        const int lg = __lg(n) + 2;
        Node<T>** A = new Node<T>*[lg];
        for (int i = 0; i < lg; i++) A[i] = nullptr;
        // iterate over all nodes w in the root list
        Node<T>* w = min;
        Node<T>* start = w;
        // debug("consolidation while start");
        while (true) {
            Node<T>* x = w;
            // debug(x->val, x->left->val, x->right->val, start->val);
            int d = x->degree;
            if (A[d] != nullptr && A[d] == x) break;
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
            w = w->right;
            if (w == start) break;
        }
        for (int i = 0; i < lg; i++) {
            if (A[i] == nullptr) continue;
            if (min == nullptr)
                min = A[i];
            else if (min->val > A[i]->val)
                min = A[i];
        }
        // debug("consolidation while end");
        assert(min != nullptr);
        delete[] A;
        debug("consolidation end");
        // printRootList(min);
    }

   public:
    FibHeap() {
        n = 0;
        min = nullptr;
    }

    ~FibHeap() {
        n = 0;
        delete[] min;
    }

    void insert(T val) {
        Node<T>* x = new Node<T>(val);
        x->left = x->right = x;
        // concatenating the root list H with the root list containing x
        // and updating the min
        if (min == nullptr) {
            min = x;
        } else {
            concatenate(min, x);
            if (x->val < min->val) min = x;
        }
        // assert(min != nullptr);
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
        assert(n > 0 && min != nullptr);
        // debug("before extraction", min->val, min->left->val, min->right->val,
        //       n);
        Node<T>* z = min;
        // for each child x of z, add x to the root list
        if (z->child != nullptr) {
            Node<T>* cur = z->child;
            while (true) {
                Node<T>* upNext = cur->right;
                cur->left = cur->right = cur;
                cur->parent = nullptr;
                z->degree--;
                concatenate(min, cur);
                cur = upNext;
                if (cur == z->child) break;
            }
        }

        // remove z from the root list
        remove(z);
        if (z == z->right) {  // only one member in the root list
            min = nullptr;
        } else {
            min = z->right;
            consolidate();
        }
        n--;
        // debug("extraction done", z->val);
        // if (min != nullptr) {
        //     debug("after extraction", min->val, min->left->val,
        //     min->right->val,
        //           n);
        // }
        return z->val;
    }
};

void dijkstra_fb(int s) {
    // initialize-single-source-distance
    fill(dist_fb.begin(), dist_fb.end(), INF);
    fill(len_fb.begin(), len_fb.end(), INF);
    fill(visited.begin(), visited.end(), false);
    dist_fb[s] = 0;
    len_fb[s] = 0;
    FibHeap<Pair> fq;
    fq.insert({s, 0});

    while (!fq.isEmpty()) {
        Pair p = fq.extractMin();
        // debug("extraction done", fq.getSize());
        int u = p.u;
        // debug(p.u, p.w, visited[p.u]);
        if (visited[u]) continue;
        visited[u] = true;
        for (auto& e : adj[u]) {
            int v = e.v;
            ll w = e.w;
            // relaxation
            if (dist_fb[v] > dist_fb[u] + w) {
                dist_fb[v] = dist_fb[u] + w;
                len_fb[v] = len_fb[u] + 1;
                fq.insert({v, dist_fb[v]});
            }
        }
    }
    // cerr << "Fibonacci done\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

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