#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

struct Pair {
    int u;
    long long w;
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
#ifdef LOCAL
#define debug(x...)               \
    cerr << "[" << #x << "] = ["; \
    _print(x)
#else
#define debug(x...)
#endif

void __print(const Pair& x) {
    cerr << '\"' << "u: " << x.u << ", w: " << x.w << '\"';
}

const long long INF = 2e16;
int n_vertices, n_edges;
vector<vector<Edge>> adj;
vector<long long> dist_bn;
vector<int> len_bn;

inline int LEFT(int x) { return x << 1; }
inline int RIGHT(int x) { return ((x << 1) | 1); }
inline int PARENT(int x) { return x >> 1; }

// Min Heap
template <typename T>
class BinHeap {
   private:
    T* arr;
    unordered_map<int, int>
        mp;  // for keeping track of indices so that O(n) search is not needed
    int maxSize, len;

    void map_swap(int idx1, int idx2) {
        int val1 = arr[idx1].u;
        int val2 = arr[idx2].u;
        mp[val1] = idx2;
        mp[val2] = idx1;
    }

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
            map_swap(idx, min_idx);
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
        // debug("before decrease", x.u, x.w, arr[idx], arr[1]);
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                map_swap(idx, parent);
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
        // debug("after decrease", x.u, x.w, arr[1]);
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
        for (int i = 0; i < maxSize; i++) {
            arr[i + 1] = vec[i];
            mp[vec[i].u] = i + 1;
        }
        len = maxSize;
        for (int i = (maxSize + 1) >> 1; i >= 1; i--) MIN_HEAPIFY(i);
    }

    BinHeap(const BinHeap& other) {
        maxSize = other.maxSize;
        len = other.len;
        delete[] arr;
        arr = new T[maxSize];
        mp.clear();
        mp = other.mp;
        for (int i = 0; i < maxSize; i++) arr[i] = other.arr[i];
    }

    ~BinHeap() {
        delete[] arr;
        mp.clear();
        len = 0;
    }

    BinHeap& operator=(const BinHeap& other) {
        if (this == &other) return *this;
        maxSize = other.maxSize;
        len = other.len;
        delete[] arr;
        arr = new T[maxSize];
        mp.clear();
        mp = other.mp;
        for (int i = 0; i < maxSize; i++) arr[i] = other.arr[i];
    }

    void insert(const T& x) {
        if (len == maxSize) {
            maxSize *= 2;
            T* another = new T[maxSize + 1];
            for (int i = 0; i <= len; i++) another[i] = arr[i];
            delete[] arr;
            arr = another;
        }
        arr[++len] = x;
        mp[x.u] = len;
        int idx = len;
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                map_swap(idx, parent);
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
    }

    void decreaseKey(const T& x, long long newVal) {
        // int idx2 = searchHelp(x);
        // assert(idx != -1);
        int idx = mp[x.u];
        assert(newVal <= arr[idx].w);
        arr[idx].w = newVal;
        decreaseKeyHelp(idx, arr[idx]);
    }

    void deleteMin() {
        assert(len > 0);
        debug(arr[1]);
        map_swap(1, len);
        swap(arr[1], arr[len]);
        mp.erase(arr[len].u);
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
    dist_bn[s] = 0;
    len_bn[s] = 0;
    vector<Pair> v(n_vertices);
    // 0-based
    for (int i = 0; i < n_vertices; i++) {
        if (i == s)
            v[i] = {i, 0};
        else
            v[i] = {i, INF};
    }
    BinHeap<Pair> pq(v);
    while (!pq.isEmpty()) {
        // extract-min
        Pair cur = pq.getMin();
        // debug(cur);
        pq.deleteMin();
        int u = cur.u;
        for (const Edge& e : adj[u]) {
            int v = e.v;
            // relaxation
            if (dist_bn[v] > dist_bn[u] + e.w) {
                pq.decreaseKey({v, dist_bn[v]}, dist_bn[u] + e.w);
                dist_bn[v] = dist_bn[u] + e.w;
                len_bn[v] = len_bn[u] + 1;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n_vertices >> n_edges;

    adj.resize(n_vertices + 2);
    dist_bn.resize(n_vertices + 2);
    len_bn.resize(n_vertices + 2);

    for (int i = 0; i < n_edges; i++) {
        int a, b;
        long long c;
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