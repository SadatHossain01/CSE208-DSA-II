#pragma once
#include <algorithm>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    double weight;
    bool operator<(const Edge& other) const {
        if (weight != other.weight) return weight < other.weight;
        return make_pair(from, to) < make_pair(other.from, other.to);
    }
};

vector<int> parent;

int find_set(int v) {  // O(logV) per call on average
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {  // O(logV) on average
    a = find_set(a);
    b = find_set(b);
    if (a != b) parent[b] = a;
}

// kruskal_ans is the vector where the resulting MST
// from this algorithm will be stored
double kruskal(vector<Edge>& edges, vector<pair<int, int>>& kruskal_ans,
               int n) {
    // O(V)
    parent.resize(n + 2);
    for (int i = 0; i <= n; i++) parent[i] = i;

    // O(E logE) or O(E logV)
    sort(edges.begin(), edges.end());

    double total_weight = 0;
    // O(E) DSU operations, per operation O(logV) on average
    for (int i = 0; i < edges.size(); i++) {
        int u = edges[i].from;
        int v = edges[i].to;
        double w = edges[i].weight;
        if (find_set(u) != find_set(v)) {
            total_weight += w;
            kruskal_ans.push_back(make_pair(u, v));
            union_sets(u, v);
        }
    }

    return total_weight;
}