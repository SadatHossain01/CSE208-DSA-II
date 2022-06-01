#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

typedef long long ll;
struct Edge {
    int from, to;
    ll weight;
};

const ll INF = 2e15;
int n, m, s, d;
vector<Edge> edges;
vector<ll> dist;
vector<int> parent;

bool bellman_ford(int source) {
    dist[source] = 0;
    parent[source] = -1;
    for (int i = 1; i <= n; i++) {
        bool changed = false;
        for (int j = 0; j < m; j++) {
            int from = edges[j].from;
            int to = edges[j].to;
            ll weight = edges[j].weight;
            if (dist[from] < INF) {
                if (dist[to] > dist[from] + weight) {
                    if (i == n) return false;
                    dist[to] = dist[from] + weight;
                    parent[to] = from;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }
    return true;
}

int main() {
    cin >> n >> m;
    edges.resize(m);
    dist.assign(n + 1, INF);
    parent.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        edges[i] = {a, b, c};
    }
    cin >> s >> d;
    bool ret = bellman_ford(s);
    if (!ret) {
        cout << "The graph contains a negative cycle\n";
        return 0;
    }
    cout << "The graph does not contain a negative cycle\n";
    cout << "Shortest path cost: " << dist[d] << "\n";
    stack<int> st;
    while (d != -1) {
        st.push(d);
        d = parent[d];
    }
    while (!st.empty()) {
        cout << st.top() << " ";
        st.pop();
        if (!st.empty()) cout << "-> ";
    }
    cout << "\n";
}