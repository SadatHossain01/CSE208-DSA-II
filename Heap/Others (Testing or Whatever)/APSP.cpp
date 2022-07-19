#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

const int INF = 2e8;
int n, m;
vector<vector<int>> adj_mat, distfw /*, parent*/;

void floyd_warshall() {
    distfw = adj_mat;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) distfw[i][i] = 0;
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                if (distfw[i][k] > distfw[i][j] + distfw[j][k]) {
                    distfw[i][k] = distfw[i][j] + distfw[j][k];
                    // if (i != k) parent[i][k] = parent[j][k];
                }
            }
        }
    }
}

int main() {
    ifstream in;
    in.open("graph.txt");

    in >> n >> m;
    adj_mat.assign(n + 2, vector<int>(n + 2, INF));
    distfw.assign(n + 2, vector<int>(n + 2, INF));
    // parent.assign(n + 2, vector<int>(n + 2, -1));

    for (int i = 0; i < m; i++) {
        int u, v, w;
        in >> u >> v >> w;
        adj_mat[u][v] = min(adj_mat[u][v], w);
        adj_mat[v][u] = min(adj_mat[v][u], w);
        // parent[u][v] = u;
    }

    floyd_warshall();

    cout << "Shortest distance matrix\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (distfw[i][j] == INF)
                cout << "INF ";
            else
                cout << distfw[i][j] << " ";
        }
        cout << "\n";
    }

    // cout << "Predecessor matrix\n";
    // for (int i = 1; i <= n; i++) {
    //     for (int j = 1; j <= n; j++) {
    //         if (parent[i][j] == -1)
    //             cout << "NIL ";
    //         else
    //             cout << parent[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

    in.close();
    return 0;
}