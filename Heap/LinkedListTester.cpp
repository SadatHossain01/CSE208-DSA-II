#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point* left;
    Point* right;
    Point(int x = 0, int y = 0) : x(x), y(y) { left = right = this; }
};

void __print(int x) { cerr << x; }
void _print() { cerr << "]\n"; }
void __print(const string& x) { cerr << '\"' << x << '\"'; }
template <typename T, typename... V>
void _print(T t, V... v) {
    __print(t);
    if (sizeof...(v)) cerr << ", ";
    _print(v...);
}
void __print(const Point& x) {
    cerr << '\"' << "u: " << x.x << ", w: " << x.y << '\"';
}
#define debug(x...)               \
    cerr << "[" << #x << "] = ["; \
    _print(x)

void printList(Point* start) {
    Point* cur = start;
    debug("printing started");
    while (true) {
        debug(*cur, *(cur->left), *(cur->right));
        cur = cur->right;
        if (cur == start) break;
    }
    debug("printing done");
}

void concatenate(Point* a, Point* b) {
    assert(a->left != nullptr && a->right != nullptr);
    assert(b->left != nullptr && b->right != nullptr);
    if (a == a->left) swap(a, b);
    if (b == b->left) {
        cout << "single\n";
        auto tmp = a->right;
        a->right = b;
        b->left = a;
        b->right = tmp;
        tmp->left = b;
    } else {
        cout << "double\n";
        a->right->left = b->left;
        b->left->right = a->right;
        a->right = b;
        b->left = a;
    }
}

void print(Point* start) {
    cout << "start\n";
    Point* cur = start;
    while (true) {
        cout << cur->x << " " << cur->y << "\n";
        cur = cur->right;
        if (cur == start) break;
    }
    cout << "end\n";
}

int main() {
    Point* head1 = new Point(2, 3);
    print(head1);
    Point* another = new Point(4, 5);
    concatenate(another, new Point(6, 7));
    concatenate(head1, another);
    printList(head1);

    Point* head2 = new Point(20, 30);
    another = new Point(40, 50);
    concatenate(another, new Point(60, 70));
    concatenate(head1, another);
    printList(head1);

    concatenate(head1, head2);
    print(head1);
}