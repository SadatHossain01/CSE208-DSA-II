#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

bool printingoff = true;

int unfixed_bound(const vector<vector<char>>& v, int fr, int fc) {
    // the first m x n part is fixed region
    // fc can be equal to or 1 more than fr
    int ans = 0;
    // first calculate for the rows
    for (int i = fr; i < v.size(); i++) {
        int non_zero_count = 0;
        for (int j = fc; j < v[0].size(); j++) non_zero_count += (v[i][j] != '0');
        ans = max(ans, (non_zero_count + 1) / 2);
    }
    // now calculate for the columns
    for (int i = fc; i < v[0].size(); i++) {
        int non_zero_count = 0;
        for (int j = fr; j < v.size(); j++) non_zero_count += (v[j][i] != '0');
        ans = max(ans, (non_zero_count + 1) / 2);
    }
    // cout << "Unfixed Region Bound: " << ans << "\n";
    return ans;
}

int fixed_bound(const vector<vector<char>>& v, int fr, int fc) {
    // 0-indexed
    //  let's first find the values of c0, c1, ...., c_(fc - 1)
    vector<int> c(fc, 0);
    for (int i = 0; i < fc; i++) {
        // diagonal element is vec[i][i]
        // first count the bottom portion of the diagonal element
        int farthest_fixed_non_zero_row = i, unfixed_non_zero = 0;
        for (int j = i + 1; j < fr; j++)
            if (v[j][i] != '0') farthest_fixed_non_zero_row = j;
        for (int j = fr; j < v.size(); j++) unfixed_non_zero += (v[j][i] != '0');
        int bottom_count = 0;
        if (unfixed_non_zero) bottom_count = fr + unfixed_non_zero - 1 - i + 1;
        else bottom_count = farthest_fixed_non_zero_row - i + 1;

        // now count the upper portion of the diagonal element
        farthest_fixed_non_zero_row = i;
        for (int j = i - 1; j >= 0; j--)
            if (v[j][i] != '0') farthest_fixed_non_zero_row = j;
        int upper_count = 0;
        upper_count = i - farthest_fixed_non_zero_row + 1;
        c[i] = max(upper_count, bottom_count);
    }

    vector<int> r(fr, 0);
    for (int i = 0; i < fr; i++) {
        // diagonal element is vec[i][i]
        // first count the right portion of the diagonal element
        int farthest_fixed_non_zero_col = i, unfixed_non_zero = 0;
        for (int j = i + 1; j < fc; j++)
            if (v[i][j] != '0') farthest_fixed_non_zero_col = j;
        for (int j = fc; j < v[0].size(); j++) unfixed_non_zero += (v[i][j] != '0');
        int right_count = 0;
        if (unfixed_non_zero) right_count = fc + unfixed_non_zero - 1 - i + 1;
        else right_count = farthest_fixed_non_zero_col - i + 1;

        // now count the left portion of the diagonal element
        farthest_fixed_non_zero_col = i;
        for (int j = i - 1; j >= 0; j--)
            if (v[i][j] != '0') farthest_fixed_non_zero_col = j;
        int left_count = 0;
        left_count = i - farthest_fixed_non_zero_col + 1;
        r[i] = max(right_count, left_count);
    }

    // for (int i = 0; i < fr; i++) cout << "r" << i + 1 << "= " << r[i] << (i == fr - 1 ? "\n" : "
    // "); for (int i = 0; i < fc; i++) cout << "c" << i + 1 << "= " << c[i] << (i == fc - 1 ? "\n"
    // : " ");

    // return max of all values
    int ret = 0;
    if (fr) ret = max(ret, *max_element(r.begin(), r.end()));
    if (fc) ret = max(ret, *max_element(c.begin(), c.end()));
    // cout << "Fixed Region Bound: " << ret << "\n";
    return ret;
}

void print(const vector<vector<char>>& mat, bool must = false) {
    if (printingoff && !must) return;
    // cout << "Current Matrix:\n";
    for (int i = 0; i < mat.size(); i++) {
        for (int j = 0; j < mat[0].size(); j++) {
            cout << mat[i][j] << " ";
        }
        cout << "\n";
    }
}

int find_bound(const vector<vector<char>>& v, int fr, int fc) {
    // print(v);
    // cout << "Fixed Row, Column: " << fr << " " << fc << "\n\n\n";
    return max(fixed_bound(v, fr, fc), unfixed_bound(v, fr, fc));
}

vector<vector<char>> moveRow(const vector<vector<char>>& v, int r, int fr, int fc) {
    // 0 to fr-1 rows will be unchanged
    // r-th row will come to fr'th place
    // and the remaining rows (which are still unfixed) will be placed serially
    vector<vector<char>> ret = v;
    // cout << "Fixed Row = " << fr << " Fixed Column = " << fc << "\n";
    // cout << "Before fixing the " << r << "-th row\n";
    // print(v);
    ret[fr] = v[r];
    int j = fr + 1;
    for (int i = fr; i < v.size(); i++) {
        if (i == r) continue;
        ret[j++] = v[i];
    }
    // cout << "After fixing the " << r << "-th row\n";
    // print(ret);
    return ret;
}

vector<vector<char>> moveColumn(const vector<vector<char>>& v, int c, int fr, int fc) {
    // 0 to fc-1 cols will be unchanged
    // c-th col will come to fc'th place
    // and the remaining cols (which are still unfixed) will be placed serially
    vector<vector<char>> ret = v;
    // cout << "Fixed Row = " << fr << " Fixed Column = " << fc << "\n";
    // cout << "Before fixing the " << c << "-th col\n";
    // print(v);
    // copying the c-th column
    for (int i = 0; i < v.size(); i++) {
        ret[i][fc] = v[i][c];
    }
    // now copying the other columns
    int j = fc + 1;
    for (int i = fc; i < v[0].size(); i++) {
        if (i == c) continue;
        for (int k = 0; k < v.size(); k++) {
            ret[k][j] = v[k][i];
        }
        j++;
    }
    // cout << "After fixing the " << c << "-th col\n";
    // print(ret);
    return ret;
}

struct Matrix {
   public:
    vector<vector<char>> mat;
    int level, order, bound;
    int fr, fc;

    Matrix(vector<vector<char>>& c, int level, int order, int fr, int fc) {
        mat = c;
        // print(mat);
        this->level = level;
        this->order = order;
        this->fr = fr;
        this->fc = fc;
        this->bound = find_bound(mat, fr, fc);
    }

    bool operator<(const Matrix& other) const {
        if (bound != other.bound) return bound > other.bound;
        else if (level != other.level) return level < other.level;
        else return order <= other.order;
    }
};

int main() {
    ifstream in;
    ofstream out;
    out.open("out.txt");
    in.open("in.txt");
    int n;
    in >> n;
    vector<vector<char>> vec(n, vector<char>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> vec[i][j];
        }
    }
    in.close();
    Matrix m(vec, 0, 1, 0, 0);
    // cout << m.bound << "\n";

    priority_queue<Matrix> pq;
    pq.push(m);
    int cont = 0;
    while (true) {
        cont++;
        Matrix now = pq.top();
        pq.pop();
        if (now.fr == n - 1 && now.fc == n - 1) {
            out << now.bound << "\n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    out << now.mat[i][j] << " ";
                }
                out << "\n";
            }
            break;
        }
        if (now.fc == now.fr) {
            // now need to fix a column
            int order = 0;
            for (int cc = now.fc; cc < n; cc++) {
                vector<vector<char>> shifted_matrix = moveColumn(now.mat, cc, now.fr, now.fc);
                Matrix mm(shifted_matrix, now.level + 1, ++order, now.fr, now.fc + 1);
                pq.push(mm);
            }
        } else {
            // now need to fix a row
            int order = 0;
            for (int rr = now.fr; rr < n; rr++) {
                vector<vector<char>> shifted_matrix = moveRow(now.mat, rr, now.fr, now.fc);
                Matrix mm(shifted_matrix, now.level + 1, ++order, now.fr + 1, now.fc);
                pq.push(mm);
            }
        }
    }
    out.close();
    cout << "Total Traversal: " << cont << "\n";
}
