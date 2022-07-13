#pragma once
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

void __print(int x) { cerr << x; }
void __print(long long x) { cerr << x; }
void _print() { cerr << "]\n"; }
void __print(const string& x) { cerr << '\"' << x << '\"'; }
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
    int u;
    long long w;
    bool operator<(const Pair& other) const { return w < other.w; }
    bool operator<=(const Pair& other) const { return w <= other.w; }
    bool operator>(const Pair& other) const { return w > other.w; }
    bool operator>=(const Pair& other) const { return w >= other.w; }
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

void __print(const Pair& x) {
    cerr << '\"' << "u: " << x.u << ", w: " << x.w << '\"';
}
