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

void make_set(int v) { parent[v] = v; }

int find_set(int v) {
    if (v == parent[v]) return v;
    return find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) parent[b] = a;
}

double kruskal(vector<Edge>& edges, vector<pair<int, int>>& kruskal_ans,
               int n) {
    parent.resize(n);
    for (int i = 0; i < n; i++) parent[i] = i;

    sort(edges.begin(), edges.end());

    double total_weight = 0;
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