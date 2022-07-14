#pragma once
#include <unordered_map>
#include <vector>

#include "header.h"
using namespace std;

inline int LEFT(int x) { return x << 1; }
inline int RIGHT(int x) { return ((x << 1) | 1); }
inline int PARENT(int x) { return x >> 1; }

// Min Heap
template <typename T>
class BinHeap {
   private:
    T* arr;
    // unordered_map<int, int> mp;
    int* mp;  // for keeping track of indices so that O(n) search is not needed
    int maxSize, len;

    void map_swap(int idx1, int idx2) {
        int val1 = arr[idx1].u;
        int val2 = arr[idx2].u;
        mp[val1] = idx2;
        mp[val2] = idx1;
    }

    void MIN_HEAPIFY(int idx) {
        if (idx < 1) return;
        int left = LEFT(idx);
        int right = RIGHT(idx);
        T minimum = arr[idx];
        int min_idx = idx;
        if (left <= len && arr[left] < minimum) {
            minimum = arr[left];
            min_idx = left;
        }
        if (right <= len && arr[right] < minimum) {
            minimum = arr[right];
            min_idx = right;
        }
        if (min_idx != idx) {
            map_swap(idx, min_idx);
            swap(arr[idx], arr[min_idx]);
            MIN_HEAPIFY(min_idx);
        }
    }

    int searchHelp(T val) {
        int ret = -1;
        for (int i = 1; i <= len; i++) {
            if (arr[i] == val) {
                ret = i;
                break;
            }
        }
        return ret;
    }

    void decreaseKeyHelp(int idx, T x) {
        // debug("before decrease", x.u, x.w, arr[idx], arr[1]);
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                map_swap(idx, parent);
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
        // debug("after decrease", x.u, x.w, arr[1]);
    }

   public:
    BinHeap(int sz = 200) {
        maxSize = sz;
        len = 0;
        arr = new T[maxSize + 1];
    }

    BinHeap(const vector<T>& vec) {
        maxSize = vec.size();
        arr = new T[maxSize + 1];
        mp = new int[maxSize + 5];
        for (int i = 0; i < maxSize; i++) {
            arr[i + 1] = vec[i];
            mp[vec[i].u] = i + 1;
        }
        len = maxSize;
        for (int i = (maxSize + 1) >> 1; i >= 1; i--) MIN_HEAPIFY(i);
    }

    BinHeap(const BinHeap& other) {
        maxSize = other.maxSize;
        len = other.len;
        delete[] arr;
        arr = new T[maxSize];
        // mp.clear();
        // mp = other.mp;
        delete[] mp;
        for (int i = 0; i < maxSize + 1; i++) arr[i] = other.arr[i];
        for (int i = 0; i < maxSize + 5; i++) mp[i] = other.mp[i];
    }

    ~BinHeap() {
        delete[] arr;
        // mp.clear();
        delete[] mp;
        len = 0;
    }

    BinHeap& operator=(const BinHeap& other) {
        if (this == &other) return *this;
        maxSize = other.maxSize;
        len = other.len;
        delete[] arr;
        arr = new T[maxSize];
        // mp.clear();
        // mp = other.mp;
        delete[] mp;
        for (int i = 0; i < maxSize + 1; i++) arr[i] = other.arr[i];
        for (int i = 0; i < maxSize + 5; i++) mp[i] = other.mp[i];
    }

    void insert(const T& x) {
        if (len == maxSize) {
            maxSize *= 2;
            T* another = new T[maxSize + 1];
            int* anotherMp = new int[maxSize + 5];
            for (int i = 0; i < maxSize + 1; i++) another[i] = arr[i];
            for (int i = 0; i < maxSize + 5; i++) anotherMp[i] = mp[i];
            delete[] arr;
            delete[] mp;
            arr = another;
            mp = anotherMp;
        }
        arr[++len] = x;
        mp[x.u] = len;
        int idx = len;
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                map_swap(idx, parent);
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
    }

    void decreaseKey(const T& x, long long newVal) {
        // int idx2 = searchHelp(x);
        // assert(idx != -1);
        int idx = mp[x.u];
        assert(newVal <= arr[idx].w);
        arr[idx].w = newVal;
        decreaseKeyHelp(idx, arr[idx]);
    }

    void deleteMin() {
        assert(len > 0);
        // debug(arr[1]);
        map_swap(1, len);
        swap(arr[1], arr[len]);
        // mp.erase(arr[len].u);
        len--;
        MIN_HEAPIFY(1);
    }

    T getMin() const {
        assert(len > 0);
        return arr[1];
    }

    int getSize() const { return len; }

    bool isEmpty() const { return len == 0; }
};