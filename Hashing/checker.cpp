#include <bits/stdc++.h>

#include "hashing.h"
using namespace std;

int N;                // guaranteed to be a prime (1000003)
double res[5][7][4];  // resolution method (in order), load factor, values

void doSeparateChaining() {
    // separate chaining
    // cout << "Separate Chaining:\n";
    // cout << "Load Factor: Time Before Deletion       Probes            Time "
    "After "
    "Deletion     Probes\n";
    int idx = 0;
    for (double lf = 0.4; lf <= 0.9; lf += 0.1, idx++) {
        auto start = chrono::high_resolution_clock::now();
        SeparateChaining sc(N);
        // cout << lf << ": ";
        int needed = lf * N;
        vector<bool> del(needed, false);
        vector<int> deleted, not_deleted;

        // generation
        vector<string> strings = generate_strings(needed, string_len);
        // debug("generation done", lf, needed);

        // insertion
        for (auto& s : strings) {
            bool ret = sc.search(s);
            if (!ret) sc.insert(s, sc.getSize() + 1);
            else {
                debug("Already present before insertion");
            }
        }
        // debug("insertion done", lf);

        vector<int> random_vector(needed);
        iota(random_vector.begin(), random_vector.end(), 0);

        // search before deletion
        int p = 0.1 * needed;
        if (p & 1) p++;
        double tot_time = 0;  // in ms
        random_shuffle(random_vector.begin(), random_vector.end());
        for (int i = 1; i <= p; i++) {
            int index = random_vector[i - 1];
            start = chrono::high_resolution_clock::now();
            bool p = sc.search(strings[index]);
            tot_time += chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0;
        }
        res[0][idx][0] = tot_time / p;
        // cout << tot_time / p << "ms              N/A               ";

        // deletion
        random_shuffle(random_vector.begin(), random_vector.end());
        for (int i = 1; i <= p; i++) {
            int index = random_vector[i - 1];
            sc.remove(strings[index]);
            del[index] = true;
        }
        for (int i = 0; i < needed; i++) {
            if (del[i]) deleted.push_back(i);
            else not_deleted.push_back(i);
        }
        // debug("deletion done", lf);

        // search after deletion
        random_shuffle(deleted.begin(), deleted.end());
        random_shuffle(not_deleted.begin(), not_deleted.end());
        tot_time = 0;
        for (int i = 1; i <= p; i++) {
            int index;
            if (i & 1) index = deleted[i / 2];    // from deleted elements
            else index = not_deleted[i / 2 - 1];  // from non-deleted items
            start = chrono::high_resolution_clock::now();
            bool p = sc.search(strings[index]);
            tot_time += chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0;
        }
        res[0][idx][2] = tot_time / p;
        // cout << tot_time / p << "ms           N/A";
        // cout << "\n";
    }
    // cout << "\n\n";
}

void doProbing(resolutionMethod p) {
    // probing
    int id2;
    if (p == LinearProbing) {
        // cout << "Linear Probing:\n";
        id2 = 1;
    } else if (p == QuadraticProbing) {
        // cout << "Quadratic Probing\n";
        id2 = 2;
    } else {
        // cout << "Double Hashing\n";
        id2 = 3;
    }
    // cout << "Load Factor: Time Before Deletion       Probes            Time "
    "After "
    "Deletion     Probes\n";
    auto start = chrono::high_resolution_clock::now();
    int idx = 0;
    for (double lf = 0.4; lf <= 0.9; lf += 0.1, idx++) {
        Probing lp(N);
        lp.setProbingMethod(p);
        ll probes;
        // cout << lf << ": ";
        int needed = lf * N;
        vector<bool> del(needed, false);
        vector<int> deleted, not_deleted;

        // generation
        vector<string> strings = generate_strings(needed, string_len);
        // debug("generation done", lf, needed);

        // insertion
        for (auto& s : strings) {
            int tmp = 0;
            bool ret = lp.search(s, tmp);
            if (!ret) lp.insert(s, lp.getSize() + 1);
            else {
                debug("Already present before insertion");
            }
        }
        // debug("insertion done", lf);

        vector<int> random_vector(needed);
        iota(random_vector.begin(), random_vector.end(), 0);

        // search before deletion
        int p = 0.1 * needed;
        if (p & 1) p++;
        double tot_time = 0;  // in micro seconds
        probes = 0;
        random_shuffle(random_vector.begin(), random_vector.end());
        for (int i = 1; i <= p; i++) {
            int index = random_vector[i - 1];
            int pp = 0;
            start = chrono::high_resolution_clock::now();
            bool p = lp.search(strings[index], pp);
            tot_time += chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0;
            probes += pp;
        }
        res[id2][idx][0] = tot_time / p;
        res[id2][idx][1] = (double)probes / p;
        // cout << tot_time / p << "ms              " << (double)probes / p << "       ";

        // deletion
        random_shuffle(random_vector.begin(), random_vector.end());
        for (int i = 1; i <= p; i++) {
            int index = random_vector[i - 1];
            lp.remove(strings[index]);
            del[index] = true;
        }
        for (int i = 0; i < needed; i++) {
            if (del[i]) deleted.push_back(i);
            else not_deleted.push_back(i);
        }
        // debug("deletion done", lf);

        // search after deletion
        tot_time = 0;
        probes = 0;
        random_shuffle(deleted.begin(), deleted.end());
        random_shuffle(not_deleted.begin(), not_deleted.end());
        for (int i = 1; i <= p; i++) {
            int index;
            if (i & 1) index = deleted[i / 2];    // from deleted elements
            else index = not_deleted[i / 2 - 1];  // from non-deleted items
            int pp = 0;
            start = chrono::high_resolution_clock::now();
            bool p = lp.search(strings[index], pp);
            tot_time += chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0;
            probes += pp;
        }
        res[id2][idx][2] = tot_time / p;
        res[id2][idx][3] = (double)probes / p;
        // cout << tot_time / p << "ms           " << (double)probes / p << "        ";
        // cout << "\n";
    }
    // cout << "\n\n";
}

const int mx = 1e7 + 5;
bitset<mx> isPrime;
vector<int> Primes;
// space optimized one is faster
void sieve(int n) {
    isPrime.set();
    isPrime[0] = isPrime[1] = false;
    for (int i = 4; i <= n; i += 2) isPrime[i] = false;
    for (int i = 3; i * i <= n; i += 2) {
        if (!isPrime[i]) continue;
        for (int j = i * i; j <= n; j += i) isPrime[j] = false;
    }
    Primes.push_back(2);
    for (int i = 3; i <= n; i += 2) {
        if (isPrime[i]) Primes.push_back(i);
    }
}

int main() {
    // time(0) returns the current time
    sieve(1000005);
    srand(time(0));

    for (int i = 0; i < Primes.size(); i++) {
        N = Primes[i];
        cerr << N << "\n";
        doSeparateChaining();
        // cerr << "Separate Chaining Done\n";
        doProbing(LinearProbing);
        // cerr << "Linear Probing Done\n";
        doProbing(QuadraticProbing);
        // cerr << "Quadratic Probing Done\n";
        doProbing(DoubleHashing);
        // cerr << "Double Hashing Done\n";
    }

    return 0;
}