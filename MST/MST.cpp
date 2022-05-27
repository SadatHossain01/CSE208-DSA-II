#include <fstream>
#include <iostream>
#include <vector>

#include "Prim.hpp"
using namespace std;

const double EPS = 1e-9;
vector<vector<pair<double, int>>> adj;
vector<pair<int, int>> prim_ans, kruskal_ans;

int main() {
    ifstream in;
    in.open("mst.txt");

    int n, m;
    in >> n >> m;
    adj.resize(n);

    for (int i = 0; i < m; i++) {
        int a, b;
        double c;
        in >> a >> b >> c;
        adj[a].push_back(make_pair(c, b));
        adj[b].push_back(make_pair(c, a));
    }

    double prim_weight = prim(adj, prim_ans, 0);

    cout << "Cost of the minimum spanning tree: " << prim_weight << endl;
    cout << "List of the edges selected by Prim's: {";
    for (int i = 0; i < prim_ans.size(); i++) {
        cout << "(" << prim_ans[i].first << "," << prim_ans[i].second << ")";
        if (i < prim_ans.size() - 1)
            cout << ",";
        else
            cout << "}" << endl;
    }
}