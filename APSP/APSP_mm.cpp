#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

const int INF = 2e8;
int n, m;
vector<vector<int>> adj_mat, distmm;

vector<vector<int>> extend_shortest_path(vector<vector<int>>& d,
                                         vector<vector<int>>& w) {
    vector<vector<int>> new_d(n + 2, vector<int>(n + 2));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            new_d[i][j] = INF;
            for (int k = 1; k <= n; k++) {
                new_d[i][j] = min(new_d[i][j], d[i][k] + w[k][j]);
            }
        }
    }

    return new_d;
}

void mat_mul_shortest_path() {
    distmm = adj_mat;

    for (int i = 2; i < n; i++) distmm = extend_shortest_path(distmm, adj_mat);
}

void mat_mul_shortest_path_faster() {
    distmm = adj_mat;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) distmm[i][i] = 0;
    }

    int b = n - 1;
    int m = 1;
    while (m < b) {
        distmm = extend_shortest_path(distmm, distmm);
        m *= 2;
    }
}

int main() {
    ifstream in;
    in.open("input.txt");

    in >> n >> m;
    adj_mat.assign(n + 2, vector<int>(n + 2, INF));
    distmm.assign(n + 2, vector<int>(n + 2, INF));

    for (int i = 1; i <= n; i++) adj_mat[i][i] = 0;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        in >> u >> v >> w;
        adj_mat[u][v] = min(adj_mat[u][v], w);
    }

    // mat_mul_shortest_path();
    mat_mul_shortest_path_faster();

    cout << "Shortest distance matrix\n";
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (distmm[i][j] == INF)
                cout << "INF ";
            else
                cout << distmm[i][j] << " ";
        }
        cout << "\n";
    }

    in.close();
    return 0;
}