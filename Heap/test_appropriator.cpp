#include <bits/stdc++.h>
using namespace std;
// convert 1-based to 0-based
int main() {
    ifstream i1;
    i1.open("test9.txt");
    ofstream o;
    o.open("large3.txt");
    int n, m;
    i1 >> n >> m;
    o << n << " " << m << "\n";
    for (int i = 0; i < m; i++) {
        int a, b, c;
        i1 >> a >> b >> c;
        a--;
        b--;
        o << a << " " << b << " " << c << "\n";
    }
    i1.close();
    o.close();
    return 0;
}