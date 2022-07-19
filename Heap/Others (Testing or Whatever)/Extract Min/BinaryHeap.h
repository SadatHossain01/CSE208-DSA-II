#pragma once
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
    int maxSize, len;

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
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
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
        for (int i = 0; i < maxSize; i++) arr[i + 1] = vec[i];
        len = maxSize;
        for (int i = (maxSize + 1) >> 1; i >= 1; i--) MIN_HEAPIFY(i);
    }

    BinHeap(const BinHeap& other) {
        maxSize = other.maxSize;
        len = other.len;
        arr = new T[maxSize];
        for (int i = 0; i < maxSize; i++) arr[i] = other.arr[i];
    }

    ~BinHeap() {
        delete[] arr;
        len = 0;
    }

    BinHeap& operator=(const BinHeap& other) {
        if (this == &other) return *this;
        maxSize = other.maxSize;
        len = other.len;
        arr = new T[maxSize];
        for (int i = 0; i < maxSize; i++) arr[i] = other.arr[i];
    }

    void insert(T x) {
        if (len == maxSize) {
            maxSize *= 2;
            T* another = new T[maxSize + 1];
            for (int i = 0; i <= len; i++) another[i] = arr[i];
            delete[] arr;
            arr = another;
        }
        arr[++len] = x;
        int idx = len;
        int parent;
        while (idx > 1) {
            parent = PARENT(idx);
            if (x < arr[parent]) {
                swap(arr[idx], arr[parent]);
                idx = parent;
            } else
                break;
        }
    }

    void decreaseKey(T x, int dx) {
        assert(dx >= 0);
        int idx = searchHelp(x);
        assert(idx != -1);
        arr[idx] = arr[idx] - dx;
        decreaseKeyHelp(idx, arr[idx]);
    }

    void deleteMin() {
        assert(len > 0);
        swap(arr[1], arr[len]);
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