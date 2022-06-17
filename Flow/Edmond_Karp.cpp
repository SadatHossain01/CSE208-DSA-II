#include <iostream>
#include <vector>
using namespace std;

struct Team {
    int id;
    string name;
    int win, loss, left;
};

int n;
vector<Team> Teams;
vector<vector<int>> against;

int main() {
    cin >> n;
    Teams.resize(n + 1);
    against.assign(n + 1, vector<int>(n + 1));
    for (int i = 0; i < n; i++) {
        cin >> Teams[i].name >> Teams[i].win >> Teams[i].loss >> Teams[i].left;
        for (int j = 0; j < n; j++) cin >> against[i][j];
    }
}