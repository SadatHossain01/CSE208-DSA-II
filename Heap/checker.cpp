#include <bits/stdc++.h>
using namespace std;
// test 6
int main() {
    ifstream i1;
    i1.open("expected (1).txt");
    vector<long long> d1(100005, 0);
    for (int i = 1; i <= 100000; i++) {
        long long x, y;
        i1 >> x;
        d1[i] = x;
    }
    i1.close();
    ifstream i2;
    i2.open("my (1).txt");
    for (int i = 1; i <= 100000; i++) {
        long long y;
        i2 >> y;
        if (y != d1[i]) {
            cout << i << " " << d1[i] << " " << y << "\n";
        }
    }
    i2.close();
    return 0;
}