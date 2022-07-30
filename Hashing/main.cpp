#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "hashing.h"
using namespace std;

int N;  // guaranteed to be a prime (1000003)

void doSeparateChaining() {
    // separate chaining
    cout << "Separate Chaining:\n";
    for (double lf = 0.4; lf <= 0.9; lf += 0.1) {
        auto start = chrono::high_resolution_clock::now();
        SeparateChaining sc(N);
        cout << "Load Factor: " << lf << "\n";
        int needed = lf * N;
        vector<bool> del(needed, false);
        vector<int> deleted, not_deleted;

        // generation
        vector<string> strings = generate_strings(needed, string_len);
        debug("generation done", lf, needed);

        // insertion
        for (auto& s : strings) sc.insert(s, sc.getSize() + 1);
        debug("insertion done", lf);

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
        debug("average search time before deletion", tot_time, tot_time / p);
        cout << "Before Deletion:\n";
        cout << "Average Search Time: " << tot_time / p << "ms\n";

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
        debug("deletion done", lf);

        // search after deletion
        tot_time = 0;
        for (int i = 1; i <= p; i++) {
            int index;
            if (i & 1) index = deleted[rand() % ((int)deleted.size())];    // from deleted elements
            else index = not_deleted[rand() % ((int)not_deleted.size())];  // from non-deleted items
            start = chrono::high_resolution_clock::now();
            bool p = sc.search(strings[index]);
            tot_time += chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0;
        }
        debug("average search time after deletion", tot_time, tot_time / p);
        cout << "After Deletion:\n";
        cout << "Average Search Time: " << tot_time / p << "ms\n";
        cout << "\n";
    }
    cout << "\n\n";
}

void doProbing(Probe p) {
    // probing
    if (p == LinearProbing) cout << "Linear Probing:\n";
    else if (p == QuadraticProbing) cout << "Quadratic Probing\n";
    else cout << "Double Hashing\n";

    auto start = chrono::high_resolution_clock::now();
    for (double lf = 0.4; lf <= 0.9; lf += 0.1) {
        Probing lp(N);
        lp.setProbingMethod(p);
        ll probes;
        cout << "Load Factor: " << lf << "\n";
        int needed = lf * N;
        vector<bool> del(needed, false);
        vector<int> deleted, not_deleted;

        // generation
        vector<string> strings = generate_strings(needed, string_len);
        debug("generation done", lf, needed);

        // insertion
        for (auto& s : strings) lp.insert(s, lp.getSize() + 1);
        debug("insertion done", lf);

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
        debug("average search time before deletion", tot_time, tot_time / p);
        debug("average number of probes", probes, (double)probes / p);
        cout << "Before Deletion:\n";
        cout << "Average Search Time: " << tot_time / p << "ms ";
        cout << "Average Number of Probes: " << (double)probes / p << "\n";

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
        debug("deletion done", lf);

        // search after deletion
        tot_time = 0;
        probes = 0;
        for (int i = 1; i <= p; i++) {
            int index;
            if (i & 1) index = deleted[rand() % ((int)deleted.size())];    // from deleted elements
            else index = not_deleted[rand() % ((int)not_deleted.size())];  // from non-deleted items
            int pp = 0;
            start = chrono::high_resolution_clock::now();
            bool p = lp.search(strings[index], pp);
            tot_time += chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0;
            probes += pp;
        }
        debug("average search time after deletion", tot_time, tot_time / p);
        debug("average number of probes", probes, (double)probes / p);
        cout << "After Deletion:\n";
        cout << "Average Search Time: " << tot_time / p << "ms ";
        cout << "Average Number of Probes: " << (double)probes / p << "\n";
        cout << "\n";
    }
    cout << "\n\n";
}

int main() {
    // time(0) returns the current time
    srand(time(0));

    // testing both hash functions
    cout << fixed << setprecision(9);
    cerr << fixed << setprecision(9);
    cout << "Hash Function 1 Performance: " << test_hash(&hash1) << "\n";
    cout << "Hash Function 2 Performance: " << test_hash(&hash2) << "\n";

    cin >> N;

    doSeparateChaining();
    doProbing(LinearProbing);
    doProbing(QuadraticProbing);
    doProbing(DoubleHashing);

    return 0;
}