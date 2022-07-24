#include <bits/stdc++.h>
using namespace std;

set<int> nums;

int main() {
    ifstream in;
    in.open("in.txt");
    ofstream out;
    out.open("result.txt");
    int n;
    in >> n;
    out << n << endl;
    for (int tc = 1; tc <= n; tc++) {
        int e, x;
        in >> e >> x;
        int res = 0;
        if (e == 0) {
            // delete
            res = nums.count(x);
            nums.erase(x);
        } else if (e == 1) {
            // insert
            res = !(nums.count(x));
            nums.insert(x);
        } else if (e == 2) {
            res = nums.count(x);
        } else if (e == 3) {
            auto it = nums.begin();
            int cnt = 0;
            while (true) {
                if (it == nums.end() || *it >= x) break;
                cnt++;
                it++;
            }
            res = cnt;
        }
        out << e << " " << x << " " << res << endl;
    }
    in.close();
    out.close();
}