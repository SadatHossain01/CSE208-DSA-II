#pragma once
#include <cstdlib>
#include <set>
#include <string>

#include "debug.h"
const int string_len = 7;
const int MOD = 1e9 + 7;
const int c1 = 1;
const int c2 = 1;

string generate_random_word(int len = string_len) {
    string ret = "";
    for (int l = 1; l <= len; l++) {
        int num = rand() % 26;
        ret.push_back('a' + num);
    }
    // debug(ret);
    return ret;
}

vector<string> generate_strings(int n, int len) {
    set<string> str;  // to ensure no word is duplicated
    vector<string> strings(n);
    int idx = 0;
    while (idx < n) {
        string s = generate_random_word(len);
        if (str.count(s)) continue;
        else {
            str.insert(s);
            strings[idx++] = s;
        }
    }
    return strings;
}

// Polynomial Rolling
unsigned long long hash1(const string& s, const int N = MOD) {
    unsigned long long hash_value = 0;
    const int n = s.size();
    const int p = 137;
    unsigned long long cur_power = 1;
    for (int i = 0; i < n; i++) {
        unsigned long long val = (s[i] - 'a' + 1) * cur_power;
        hash_value += val;
        cur_power = cur_power * p;
    }
    // debug(s, hash_value);
    return hash_value;
}

// Jenkins Hash Function
unsigned long long hash2(const string& s, const int N = MOD) {
    // https://en.wikipedia.org/wiki/Jenkins_hash_function
    unsigned long long hash_value = 0;
    const int n = s.size();
    for (int i = 0; i < n; i++) {
        hash_value += s[i];
        hash_value += (hash_value << 10);
        hash_value ^= (hash_value >> 6);
    }
    hash_value += (hash_value << 3);
    hash_value ^= (hash_value >> 11);
    hash_value += (hash_value << 15);
    return hash_value % N;
}

double test_hash(unsigned long long (*func)(const string&, const int N)) {
    set<string> str;
    set<unsigned long long> hash_values;
    int dataset_size = 100;
    while (str.size() < dataset_size) {
        string s = generate_random_word(string_len);
        str.insert(s);
    }
    for (auto& s : str) hash_values.insert(func(s, MOD));
    double rate = ((int)hash_values.size() * 1.00) / dataset_size;
    debug(rate);
    return rate;
}

struct Pair {
   public:
    string key;
    int value;
    Pair* prev;
    Pair* next;
    Pair(const string& key, int value) : key(key), value(value) { prev = next = nullptr; }
};

class SeparateChaining {
   private:
    int m, size = 0;
    Pair** hashTable;
    // using h1 as the hash function here

    Pair* search_help(const string& s) {
        int h = hash1(s) % m;
        if (hashTable[h] == nullptr) return nullptr;
        Pair* cur = hashTable[h];
        while (cur != nullptr) {
            if (cur->key == s) return cur;
            else cur = cur->next;
        }
        return nullptr;
    }

   public:
    SeparateChaining(int m) {
        this->m = m;
        hashTable = new Pair*[m];
        for (int i = 0; i < m; i++) hashTable[i] = nullptr;
    }

    ~SeparateChaining() {
        for (int i = 0; i < m; i++) {
            if (hashTable[i] == nullptr) continue;
            while (hashTable[i]->next != nullptr) {
                Pair* temp = hashTable[i]->next;
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
            }
        }
        delete hashTable;
    }

    int getSize() { return size; }

    bool search(const string& s) {
        Pair* ret = search_help(s);
        if (ret == nullptr) return false;
        else return true;
    }

    void insert(const string& s, int val) {
        // first check if this string is already in the hash table
        Pair* ret = search_help(s);
        if (ret != nullptr) return;  // already present

        int h = hash1(s) % m;
        Pair* cur = hashTable[h];
        Pair* toBeInserted = new Pair(s, val);
        if (cur == nullptr) cur = toBeInserted;
        else {
            while (cur->next != nullptr) cur = cur->next;
            cur->next = toBeInserted;
            toBeInserted->prev = cur;
        }
        size++;
    }

    void remove(const string& s) {
        Pair* ret = search_help(s);
        if (ret == nullptr) return;  // not present

        int h = hash1(s) % m;
        if (ret == hashTable[h]) {  // in head
            hashTable[h] = ret->next;
            if (hashTable[h] != nullptr) hashTable[h]->prev = nullptr;
        } else {
            Pair* after = ret->next;
            Pair* before = ret->prev;
            if (after != nullptr) after->prev = before;
            before->next = after;  // as ret is not head, so before is surely not null
        }
        delete ret;
    }
};

struct Pair2 {
   public:
    string key;
    int value;
    Pair2() : key(""), value(-1) {}
    Pair2(const string& key, int value) : key(key), value(value) {}
};

enum Probe { LinearProbing, QuadraticProbing, DoubleHashing };

class Probing {
   private:
    int m, size;
    Pair2* hashTable;
    Probe p;
    bool* deleted;

    int hash(const string& s, int i) {
        if (p == LinearProbing) return (hash1(s) + i) % m;
        else if (p == QuadraticProbing) return (hash1(s) + c1 * i + c2 * i * i) % m;
        else return (hash1(s) + i * hash2(s)) % m;
    }

    int search_help(const string& s, int& probe) {
        // returns the found index, else -1
        probe = 0;
        while (probe < m) {
            int idx = hash(s, probe);
            if (hashTable[idx].key == s) return idx;
            else if (!deleted[idx] && hashTable[idx].value == -1) return -1;
            probe++;
        }
        return -1;
    }

   public:
    void setProbingMethod(Probe p) { this->p = p; }

    Probing(int m) {
        this->m = m;
        hashTable = new Pair2[m];
        deleted = new bool[m];
        for (int i = 0; i < m; i++) deleted[i] = false;
    }

    int getSize() { return size; }

    bool search(const string& s, int& p) {
        int pr = 0;
        int ret = search_help(s, pr);
        pr++;  // started from 0 there
        p = pr;
        if (ret == -1) return false;
        else return true;
    }

    void insert(const string& s, int val) {
        int temp = 0;
        if (search(s, temp))  // already present
            return;

        int i = 0;
        while (i < m) {
            int j = hash(s, i);
            if (hashTable[j].value == -1) {
                hashTable[j].key = s;
                hashTable[j].value = val;
                deleted[j] = false;
                size++;
                return;
            } else i++;
        }
    }

    void remove(const string& s) {
        int temp = 0;
        int ret = search(s, temp);
        if (ret == -1) return;  // not present
        hashTable[ret].key = "";
        hashTable[ret].value = -1;
        deleted[ret] = true;
    }
};