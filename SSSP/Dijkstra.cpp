#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

typedef long long ll;
struct Edge {
    int to;
    ll weight;
    bool operator>(const Edge& other) const {
        if (weight != other.weight) return weight > other.weight;
        return to > other.to;
    }
};

const ll INF = 2e15;
int n, m, s, d;
vector<vector<Edge>> adj;
vector<ll> dist;
vector<int> parent;
vector<bool> visited;

void dijkstra(int source) {
    parent[source] = -1;
    dist[source] = 0;
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
    pq.push({source, 0});
    while (!pq.empty()) {
        int from = pq.top().to;
        pq.pop();
        if (visited[from]) continue;
        visited[from] = true;
        for (auto& edge : adj[from]) {
            if (dist[edge.to] > dist[from] + edge.weight) {
                dist[edge.to] = dist[from] + edge.weight;
                parent[edge.to] = from;
                pq.push({edge.to, dist[edge.to]});
            }
        }
    }
}

int main() {
    ifstream in;
    in.open("d_in.txt");
    in >> n >> m;
    adj.resize(n + 1);
    dist.assign(n + 1, INF);
    parent.resize(n + 1);
    visited.assign(n + 1, false);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        in >> a >> b >> c;
        adj[a].push_back({b, c});
    }
    in >> s >> d;
    dijkstra(s);
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
    in.close();
}