#pragma once
#include <iostream>
using namespace std;

void __print(int x) { cerr << x; }
void _print() { cerr << "]\n"; }
template <typename T, typename... V>
void _print(T t, V... v) {
    __print(t);
    if (sizeof...(v)) cerr << ", ";
    _print(v...);
}
#ifdef LOCAL
#define debug(x...)               \
    cerr << "[" << #x << "] = ["; \
    _print(x)
#else
#define debug(x...)
#endif

struct Pair {
    int u, w;
    bool operator<(const Pair& other) const { return w <= other.w; }
    bool operator>(const Pair& other) const { return w > other.w; }
    bool operator==(const Pair& other) const {
        return u == other.u && w == other.w;
    }
    Pair operator-(int dx) const {
        Pair ret = {u, w - dx};
        return ret;
    }
    Pair& operator=(const Pair& other) {
        u = other.u;
        w = other.w;
        return *this;
    }
};
