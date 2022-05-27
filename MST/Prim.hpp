#pragma once
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

const double INF = 2e9;

typedef pair<double, pair<int, int>> dii;

// adj is a 2D vector, in which an element <w, j>
// in the i-th row denotes an edge from i to j with weight w
// prim_ans is the vector where the resulting MST
// from this algorithm will be stored
double prim(vector<vector<pair<double, int>>>& adj,
            vector<pair<int, int>>& prim_ans, int source) {
    int n = adj.size();
    vector<bool> taken(n, false);  // if a vertex is included in the MST
    vector<double> key(n, INF);
    priority_queue<dii, vector<dii>, greater<dii>> pq;  // {weight, {from, to}}
    pq.push(make_pair(0, make_pair(source, source)));
    key[source] = 0;
    double total_weight = 0;

    while (!pq.empty()) {
        dii ret = pq.top();
        pq.pop();

        int u = ret.second.second;  // vertex with the minimum key
        double w = ret.first;
        if (taken[u]) continue;  // u already included in MST
        taken[u] = true;         // include u in MST

        if (ret.second.first !=
            ret.second.second) {  // to ignore the first popped pair (0, 0)
            total_weight += w;
            prim_ans.push_back(ret.second);
        }

        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].second;
            double weight = adj[u][i].first;
            if (!taken[v] && key[v] > weight) {
                key[v] = weight;  // decrease-key operation
                pq.push(make_pair(weight, make_pair(u, v)));
                // there might be cases where multiple edges with same ending
                // point might be in the queue. But checking if taken[u]
                // is true, discards those additional pushes
            }
        }
    }

    return total_weight;
}