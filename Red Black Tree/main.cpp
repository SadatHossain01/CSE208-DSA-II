#include <fstream>
#include <iostream>

#include "rbtree.h"
using namespace std;

int main() {
    RBT<int> tree;
    ifstream in;
    in.open("in.txt");
    int n;
    in >> n;
    cout << n << endl;
    for (int tc = 1; tc <= n; tc++) {
        int e, x;
        in >> e >> x;
        int res = 0;
        if (e == 0) {
            // delete
            res = tree.remove(x);
        } else if (e == 1) {
            // insert
            res = tree.insert(x);
        } else if (e == 2) {
            // search
            res = tree.search(x);
        } else if (e == 3) {
            // lower count
            res = tree.lower_count(x);
        }
        // cout << tc << endl;
        cout << e << " " << x << " " << res << endl;
    }
    // cout << "done" << endl;
    return 0;
}