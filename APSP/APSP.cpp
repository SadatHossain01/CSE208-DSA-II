#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

const int INF = 2e8;
int n, m;
vector<vector<int>> adj_mat, dist;

void floyd_warshall() {
    dist = adj_mat;

    for (int j = 1; j <= n; j++) {
        for (int i = 1; i <= n; i++) {
            for (int k = 1; k <= n; k++) {
                dist[i][k] = min(dist[i][k], dist[i][j] + dist[j][k]);
            }
        }
    }
}

vector<vector<int>> extend_shortest_path(vector<vector<int>>& d) {
    vector<vector<int>> new_d(n + 2, vector<int>(n + 2));

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            new_d[i][j] = INF;
            for (int k = 1; k <= n; k++) {
                new_d[i][j] = min(new_d[i][j], d[i][k] + adj_mat[k][j]);
            }
        }
    }

    return new_d;
}

void mat_mul_shortest_path() {
    dist = adj_mat;

    for (int i = 2; i < n; i++) {
        vector<vector<int>> d = extend_shortest_path(dist);
        dist = d;
    }
}

int main() {
    ifstream in;
    in.open("input.txt");

    in >> n >> m;
    adj_mat.assign(n + 2, vector<int>(n + 2, INF));
    dist.assign(n + 2, vector<int>(n + 2, INF));

    for (int i = 1; i <= n; i++) adj_mat[i][i] = 0;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        in >> u >> v >> w;
        adj_mat[u][v] = w;
    }

    floyd_warshall();
    // mat_mul_shortest_path();

    cout << "Shortest distance matrix\n";
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (dist[i][j] == INF)
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << "\n";
    }

    in.close();
    return 0;
}