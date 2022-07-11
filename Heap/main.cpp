#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "BinaryHeap.h"
#include "header.h"
using namespace std;

struct Edge {
    int u, v, w;
};

const int INF = 2e8;
int n_vertices, n_edges, k;
vector<vector<Edge>> adj;
vector<int> dist_bn, dist_fb;
vector<int> len_bn, len_fb;

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
    while (pq.getLen() > 0) {
        // extract-min
        Pair cur = pq.getMin();
        // debug(cur.u, cur.w);
        pq.deleteMin();
        int u = cur.u;
        for (const Edge& e : adj[u]) {
            int v = e.v;
            // relaxation
            if (dist_bn[v] > dist_bn[u] + e.w) {
                int dx = dist_bn[v] - dist_bn[u] - e.w;
                pq.decreaseKey({v, dist_bn[v]}, dx);
                dist_bn[v] -= dx;
                len_bn[v] = len_bn[u] + 1;
            }
        }
    }
}

int main() {
    ifstream in1;
    in1.open("graph.txt");

    in1 >> n_vertices >> n_edges;
    adj.resize(n_vertices + 1);
    dist_bn.resize(n_vertices + 1);
    dist_fb.resize(n_vertices + 1);
    len_bn.resize(n_vertices + 1);
    len_fb.resize(n_vertices + 1);
    for (int i = 1; i <= n_edges; i++) {
        int u, v, w;
        in1 >> u >> v >> w;
        Edge e = {u, v, w};
        adj[u].push_back(e);
        e = {v, u, w};
        adj[v].push_back(e);
    }

    in1.close();

    ifstream in2;
    in2.open("queries.txt");
    ofstream out;
    out.open("output.txt");
    in2 >> k;
    for (int q = 1; q <= k; q++) {
        int s, t;
        in2 >> s >> t;
        auto startTime = chrono::high_resolution_clock::now();
        dijkstra_bn(s);
        auto endTime = chrono::high_resolution_clock::now();
        double binary_time =
            chrono::duration_cast<chrono::nanoseconds>(endTime - startTime)
                .count() /
            (1000000.0);
        cout << len_bn[t] << " " << dist_bn[t] << " " << binary_time << "\n";
    }

    in2.close();
    out.close();
    return 0;
}