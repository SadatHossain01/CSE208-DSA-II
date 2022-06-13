#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

const int INF = 2e8;
int n, m;
vector<vector<int>> adj_mat, distfw;

void floyd_warshall() {
    distfw = adj_mat;

    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= n; i++) {
            for (int k = 1; k <= n; k++) {
                distfw[i][k] = min(distfw[i][k], distfw[i][j] + distfw[j][k]);
            }
        }
    }
}

int main() {
    ifstream in;
    in.open("input.txt");

    in >> n >> m;
    adj_mat.assign(n + 2, vector<int>(n + 2, INF));
    distfw.assign(n + 2, vector<int>(n + 2, INF));

    for (int i = 1; i <= n; i++) adj_mat[i][i] = 0;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        in >> u >> v >> w;
        adj_mat[u][v] = min(adj_mat[u][v], w);
    }

    floyd_warshall();

    cout << "Shortest distance matrix\n";
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (distfw[i][j] == INF)
                cout << "INF ";
            else
                cout << distfw[i][j] << " ";
        }
        cout << "\n";
    }

    in.close();
    return 0;
}