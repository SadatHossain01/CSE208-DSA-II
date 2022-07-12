#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream in1;
    in1.open("large_in.txt");
    ofstream out;
    out.open("in2.txt");
    int n, m;
    in1 >> n >> m;
    out << n << " " << m << "\n";
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        in1 >> u >> v >> w;
        u--;
        v--;
        out << u << " " << v << " " << w << "\n";
    }
    in1.close();
}