#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "Kruskal.hpp"
#include "Prim.hpp"
using namespace std;

const double EPS = 1e-9;
// Edge struct contains from, Edge2 does not
vector<vector<Edge2>> adj;  // for Prim
vector<Edge> edges;         // for Kruskal
vector<pair<int, int>> prim_ans, kruskal_ans;

int main() {
    ifstream in;
    in.open("mst.txt");

    int n, m;
    in >> n >> m;
    adj.resize(n + 1);
    edges.resize(m);

    for (int i = 0; i < m; i++) {
        int a, b;
        double c;
        in >> a >> b >> c;
        Edge2 e;
        e.to = b;
        e.weight = c;
        adj[a].push_back(e);
        e.to = a;
        adj[b].push_back(e);
        edges[i].from = a;
        edges[i].to = b;
        edges[i].weight = c;
    }

    double prim_weight = prim(adj, prim_ans, n);
    double kruskal_weight = kruskal(edges, kruskal_ans, n);

    // assert(abs(prim_weight - kruskal_weight) < EPS && prim_ans.size() == n -
    // 1 && kruskal_ans.size() == n - 1);

    cout << "Cost of the minimum spanning tree: " << prim_weight << endl;

    cout << "List of the edges selected by Prim's: {";
    for (int i = 0; i < prim_ans.size(); i++) {
        cout << "(" << prim_ans[i].first << "," << prim_ans[i].second << ")";
        if (i < prim_ans.size() - 1)
            cout << ",";
        else
            cout << "}" << endl;
    }

    cout << "List of the edges selected by Kruskal's: {";
    for (int i = 0; i < kruskal_ans.size(); i++) {
        cout << "(" << kruskal_ans[i].first << "," << kruskal_ans[i].second
             << ")";
        if (i < kruskal_ans.size() - 1)
            cout << ",";
        else
            cout << "}" << endl;
    }

    in.close();
}