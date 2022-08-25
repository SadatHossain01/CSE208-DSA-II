#include <bits/stdc++.h>

#include "main.cpp"
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<char>> vec(n, vector<char>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> vec[i][j];
        }
    }

    cout << fixed_bound(vec, 2, 3) << endl;
}