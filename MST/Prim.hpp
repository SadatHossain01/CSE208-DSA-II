#pragma once
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

const double INF = 2e9;

struct Edge2 {
    int to;
    double weight;
    bool operator<(const Edge2& other) const {
        if (weight != other.weight) return weight < other.weight;
        return to < other.to;
    }
};

// prim_ans is the vector to store the resulting MST
// taken records if a node has been included in the MST
double primHelp(vector<vector<Edge2>>& adj, vector<bool>& taken,
                vector<double>& key, vector<pair<int, int>>& prim_ans,
                int source) {
    priority_queue<pair<Edge2, int>, vector<pair<Edge2, int>>,
                   greater<pair<Edge2, int>>>
        pq;
    // <{to, weight}, from>
    Edge2 temp;
    temp.to = source;
    temp.weight = 0;
    pq.push(make_pair(temp, source));
    key[source] = 0;
    double total_weight = 0;

    while (!pq.empty()) {  // O(E) pops at most
        pair<Edge2, int> ret = pq.top();
        pq.pop();  // per pop is O(logE)

        int u = ret.first.to;  // vertex with the minimum key from the p_queue
        double w = ret.first.weight;
        if (taken[u]) continue;  // u already included in MST
        taken[u] = true;         // include u in MST

        if (ret.second !=
            ret.first.to) {  // to ignore the first popped pair (source, source)
            total_weight += w;
            prim_ans.push_back(make_pair(ret.second, ret.first.to));
        }

        for (int i = 0; i < adj[u].size(); i++) {  // will run O(E) in total
            int v = adj[u][i].to;
            double weight = adj[u][i].weight;
            if (!taken[v] && key[v] > weight) {
                key[v] = weight;  // decrease-key operation
                Edge2 temp;
                temp.to = v;
                temp.weight = weight;
                pq.push(make_pair(temp, u));
                // there might be cases where multiple edges with same ending
                // point might be in the queue. But checking taken[u]'s truth
                // discards those additional pushes
            }
        }
    }

    return total_weight;
}

// for a disconnected graph, there will be multiple MSTs resulting in
// Minimum Spanning Forest. We want to go through every node, and if a node
// is not already part of an MST, run Prim's from it
double prim(vector<vector<Edge2>>& adj, vector<pair<int, int>>& prim_ans,
            int n) {
    // 0-based
    vector<bool> taken(n + 1, false);  // if a vertex is included in the MST
    vector<double> key(n + 1, INF);
    double ans = 0;
    for (int i = 0; i < n; i++) {
        if (!taken[i]) ans += primHelp(adj, taken, key, prim_ans, i);
    }
    return ans;
}