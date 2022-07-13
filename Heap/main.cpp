#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "BinaryHeap.h"
#include "FibHeap.h"
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
vector<bool> visited;

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
    cerr << "Binary done\n";
}

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
            int w = e.w;
            // relaxation
            if (dist_fb[v] > dist_fb[u] + w) {
                dist_fb[v] = dist_fb[u] + w;
                len_fb[v] = len_fb[u] + 1;
                fq.insert({v, dist_fb[v]});
            }
        }
    }
    cerr << "Fibonacci done\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ifstream in1;
    in1.open("large1.txt");

    in1 >> n_vertices >> n_edges;
    adj.resize(n_vertices + 2);
    dist_bn.resize(n_vertices + 2);
    dist_fb.resize(n_vertices + 2);
    len_bn.resize(n_vertices + 2);
    len_fb.resize(n_vertices + 2);
    visited.resize(n_vertices + 2);
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
    in2.open("q_large1.txt");
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
        cout << "Binary: " << dist_bn[t] << "\n";
        startTime = chrono::high_resolution_clock::now();
        dijkstra_fb(s);
        endTime = chrono::high_resolution_clock::now();
        double fibonacci_time =
            chrono::duration_cast<chrono::nanoseconds>(endTime - startTime)
                .count() /
            (1000000.0);
        // debug(len_fb[t], len_bn[t], dist_fb[t], dist_bn[t]);
        cout << "Fibonacci: " << dist_fb[t] << "\n";
        cout << len_bn[t] << " " << dist_bn[t] << " " << binary_time << "ms "
             << fibonacci_time << "ms\n";
    }

    in2.close();
    out.close();
    return 0;
}