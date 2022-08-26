#include <bits/stdc++.h>
using namespace std;

int unfixed_bound(const vector<vector<char>>& vec, int fixed_r, int fixed_c) {
    // the first m x n part is fixed region
    // fixed_c can be equal to or 1 more than fixed_r
    int ans = 0;
    // first calculate for the rows
    for (int i = fixed_r; i < vec.size(); i++) {
        int non_zero_count = 0;
        for (int j = fixed_c; j < vec[0].size(); j++) non_zero_count += (vec[i][j] != '0');
        ans = max(ans, (non_zero_count + 1) / 2);
    }
    // now calculate for the columns
    for (int i = fixed_c; i < vec[0].size(); i++) {
        int non_zero_count = 0;
        for (int j = fixed_r; j < vec.size(); j++) non_zero_count += (vec[j][i] != '0');
        ans = max(ans, (non_zero_count + 1) / 2);
    }
    return ans;
}

int fixed_bound(const vector<vector<char>>& vec, int fixed_r, int fixed_c) {
    // 0-indexed
    //  let's first find the values of c0, c1, ...., c_(fixed_c - 1)
    vector<int> c(fixed_c, 0);
    for (int i = 0; i < fixed_c; i++) {
        // diagonal element is vec[i][i]
        // first count the bottom portion of the diagonal element
        int farthest_fixed_non_zero_row = i, unfixed_non_zero = 0;
        for (int j = i + 1; j < fixed_r; j++)
            if (vec[j][i] != '0') farthest_fixed_non_zero_row = j;
        for (int j = fixed_r; j < vec.size(); j++) unfixed_non_zero += (vec[j][i] != '0');
        int bottom_count = 0;
        if (unfixed_non_zero) bottom_count = fixed_r + unfixed_non_zero - 1 - i + 1;
        else bottom_count = farthest_fixed_non_zero_row - i + 1;

        // now count the upper portion of the diagonal element
        farthest_fixed_non_zero_row = i;
        for (int j = i - 1; j >= 0; j--)
            if (vec[j][i] != '0') farthest_fixed_non_zero_row = j;
        int upper_count = 0;
        upper_count = i - farthest_fixed_non_zero_row + 1;
        c[i] = max(upper_count, bottom_count);
    }

    vector<int> r(fixed_r, 0);
    for (int i = 0; i < fixed_r; i++) {
        // diagonal element is vec[i][i]
        // first count the right portion of the diagonal element
        int farthest_fixed_non_zero_col = i, unfixed_non_zero = 0;
        for (int j = i + 1; j < fixed_c; j++)
            if (vec[i][j] != '0') farthest_fixed_non_zero_col = j;
        for (int j = fixed_c; j < vec[0].size(); j++) unfixed_non_zero += (vec[i][j] != '0');
        int right_count = 0;
        if (unfixed_non_zero) right_count = fixed_c + unfixed_non_zero - 1 - i + 1;
        else right_count = farthest_fixed_non_zero_col - i + 1;

        // now count the left portion of the diagonal element
        farthest_fixed_non_zero_col = i;
        for (int j = i - 1; j >= 0; j--)
            if (vec[i][j] != '0') farthest_fixed_non_zero_col = j;
        int left_count = 0;
        left_count = i - farthest_fixed_non_zero_col + 1;
        r[i] = max(right_count, left_count);
    }

    for (int i = 0; i < fixed_r; i++) cerr << "r" << i + 1 << ": " << r[i] << "\n";
    cerr << "\n";
    for (int i = 0; i < fixed_c; i++) cerr << "c" << i + 1 << ": " << c[i] << "\n";
    cerr << "\n";

    // return max of all values
    return max(*max_element(c.begin(), c.end()), *max_element(r.begin(), r.end()));
}

int bound(const vector<vector<char>>& vec, int fixed_r, int fixed_c) {
    return max(fixed_bound(vec, fixed_r, fixed_c), unfixed_bound(vec, fixed_r, fixed_c));
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<char>> vec(n, vector<char>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> vec[i][j];
        }
    }
    int fr, fc;
    cin >> fr >> fc;
    cout << fixed_bound(vec, fr, fc) << endl;
}