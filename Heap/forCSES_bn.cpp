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
vector<ll> dist_bn;
vector<int> len_bn;
vector<bool> visited;

inline int LEFT(int x) { return x << 1; }
inline int RIGHT(int x) { return ((x << 1) | 1); }
inline int PARENT(int x) { return x >> 1; }

// Min Heap
template <typename T>
class BinHeap {
   private:
    T* arr;
    int maxSize, len;

    void MIN_HEAPIFY(int idx) {
        if (idx < 1) return;
        int left = LEFT(idx);
        int right = RIGHT(idx);
        T minimum = arr[idx];
        int min_idx = idx;
        if (left <= len && arr[left] < minimum) {
            minimum = arr[left];
            min_idx = left;
        }
        if (right <= len && arr[right] < minimum) {
            minimum = arr[right];
            min_idx = right;
        }
        if (min_idx != idx) {
            swap(arr[idx], arr[min_idx]);
            MIN_HEAPIFY(min_idx);
        }
    }

    int searchHelp(T val) {
        int ret = -1;
        for (int i = 1; i <= len; i++) {
            if (arr[i] == val) {
                ret = i;
                break;
            }
        }
        return ret;
    }

    void decreaseKeyHelp(int idx, T x) {
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
    }

   public:
    BinHeap(int sz = 200) {
        maxSize = sz;
        len = 0;
        arr = new T[maxSize + 1];
    }

    BinHeap(const vector<T>& vec) {
        maxSize = vec.size();
        arr = new T[maxSize + 1];
        for (int i = 0; i < maxSize; i++) arr[i + 1] = vec[i];
        len = maxSize;
        for (int i = (maxSize + 1) >> 1; i >= 1; i--) MIN_HEAPIFY(i);
    }

    BinHeap(const BinHeap& other) {
        maxSize = other.maxSize;
        len = other.len;
        arr = new T[maxSize];
        for (int i = 0; i < maxSize; i++) arr[i] = other.arr[i];
    }

    ~BinHeap() {
        delete[] arr;
        len = 0;
    }

    BinHeap& operator=(const BinHeap& other) {
        if (this == &other) return *this;
        maxSize = other.maxSize;
        len = other.len;
        arr = new T[maxSize];
        for (int i = 0; i < maxSize; i++) arr[i] = other.arr[i];
    }

    void insert(T x) {
        if (len == maxSize) {
            maxSize *= 2;
            T* another = new T[maxSize + 1];
            for (int i = 0; i <= len; i++) another[i] = arr[i];
            delete[] arr;
            arr = another;
        }
        arr[++len] = x;
        int idx = len;
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
    }

    void decreaseKey(T x, int dx) {
        assert(dx >= 0);
        int idx = searchHelp(x);
        assert(idx != -1);
        arr[idx] = arr[idx] - dx;
        decreaseKeyHelp(idx, arr[idx]);
    }

    void deleteMin() {
        assert(len > 0);
        swap(arr[1], arr[len]);
        len--;
        MIN_HEAPIFY(1);
    }

    T getMin() const {
        assert(len > 0);
        return arr[1];
    }

    int getSize() const { return len; }

    bool isEmpty() const { return len == 0; }
};

void dijkstra_bn(int s) {
    // initialize-single-source-distance
    fill(dist_bn.begin(), dist_bn.end(), INF);
    fill(len_bn.begin(), len_bn.end(), INF);
    fill(visited.begin(), visited.end(), false);
    dist_bn[s] = 0;
    len_bn[s] = 0;
    BinHeap<Pair> bq;
    bq.insert({s, 0});

    while (!bq.isEmpty()) {
        Pair p = bq.getMin();
        int u = p.u;
        // debug(p.u, p.w);
        bq.deleteMin();
        if (visited[u]) continue;
        visited[u] = true;
        for (auto& e : adj[u]) {
            int v = e.v;
            int w = e.w;
            // relaxation
            if (dist_bn[v] > dist_bn[u] + w) {
                dist_bn[v] = dist_bn[u] + w;
                len_bn[v] = len_bn[u] + 1;
                bq.insert({v, dist_bn[v]});
            }
        }
    }
    // cerr << "Binary done\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n_vertices >> n_edges;

    adj.resize(n_vertices + 2);
    dist_bn.resize(n_vertices + 2);
    len_bn.resize(n_vertices + 2);
    visited.resize(n_vertices + 2);

    for (int i = 0; i < n_edges; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        Edge e = {a, b, c};
        adj[a].push_back(e);
    }

    dijkstra_bn(0);

    for (int i = 0; i < n_vertices; i++) {
        cout << dist_bn[i] << " ";
    }
}