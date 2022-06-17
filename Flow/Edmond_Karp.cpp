#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Team {
    int nodeID;
    string name;
    int win, loss, left;
};

struct Node {
    int id;  // node id to be used for indexing, source id 0, sink id 1
    int team1 = -1, team2 = -1;
};

int numberOfTeams, numberOfNodes;
int total_matches = 0;
int maxPossibleWin = 0;
int maxPossibleWinningID = -1;
Node s = {0}, t = {1};
vector<Team> Teams;
vector<vector<int>> mAgainst;
vector<vector<int>> teamPairID;  // stores the node id for the pair of teams
vector<vector<int>> capacity;
vector<vector<Node>> adj;
vector<bool> visited;

void generateNodeIDs() {
    numberOfNodes = 2;  // as source node id 0 and sink node id 1
    // generating node id for the pair of teams
    for (int i = 0; i < numberOfTeams; i++) {
        for (int j = i + 1; j < numberOfTeams; j++) {
            teamPairID[i][j] = numberOfNodes++;
            teamPairID[j][i] = teamPairID[i][j];
        }
    }

    // generating node id for the teams
    for (int i = 0; i < numberOfTeams; i++) {
        Teams[i].nodeID = numberOfNodes++;
    }
}

void buildGraph() {
    adj.resize(numberOfNodes + 1);
    capacity.assign(numberOfNodes + 1, vector<int>(numberOfNodes + 1, 0));
    visited.assign(numberOfNodes + 1, false);

    for (int i = 0; i < numberOfTeams; i++) {
        for (int j = i + 1; j < numberOfTeams; j++) {
            int pairID = teamPairID[i][j];

            // every edge is also reversed for residual graph
            //  edge between source and all pairs of teams
            adj[s.id].push_back({pairID, i, j});
            adj[pairID].push_back({s.id});

            // edge between a pair of teams and those teams' respective nodes
            int team1ID = Teams[i].nodeID;
            int team2ID = Teams[j].nodeID;

            adj[pairID].push_back({team1ID, i, i});
            adj[team1ID].push_back({pairID, i, j});

            adj[pairID].push_back({team2ID, j, j});
            adj[team2ID].push_back({pairID, i, j});
        }
    }

    // there should also be edges between the team nodes and the sink
    for (int i = 0; i < numberOfTeams; i++) {
        int teamID = Teams[i].nodeID;
        adj[teamID].push_back({t.id});
        adj[t.id].push_back({teamID, i, i});
    }
}

void setCapacity(int x) {
    for (int i = 0; i < numberOfTeams; i++) {
        for (int j = i + 1; j < numberOfTeams; j++) {
            int pairID = teamPairID[i][j];

            // capacity of the edge between source and this pair node will be
            // the # of matches played between these two teams
            capacity[s.id][pairID] = mAgainst[i][j];
            capacity[pairID][s.id] = 0;

            // edge between a pair of teams and those teams' respective nodes
            int team1ID = Teams[i].nodeID;
            int team2ID = Teams[j].nodeID;

            // an infinite capacity can be set for the edge between a pair and a
            // team as that is anyways determined by the capacity of the
            // incoming edge to the pair node
            capacity[pairID][team1ID] = total_matches;
            capacity[team1ID][pairID] = 0;
            capacity[pairID][team2ID] = total_matches;
            capacity[team2ID][pairID] = 0;
        }
    }

    for (int i = 0; i < numberOfTeams; i++) {
        int teamID = Teams[i].nodeID;
        // x can win at most x.win + x.left matches, so in order for team i to
        // win at most that, it can win x.win + x.left - i.win more matches
        capacity[teamID][t.id] = Teams[x].win + Teams[x].left - Teams[i].win;
        capacity[t.id][teamID] = 0;
    }
}

int bfs(vector<int>& parent, int source, int sink) {
    parent.assign(numberOfNodes + 1, -1);

    parent[source] = -2;
    queue<pair<int, int>> q;
    // we can use total_matches as the infinite flow
    q.push({source, total_matches});

    while (!q.empty()) {
        int now = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (auto& other : adj[now]) {
            if (parent[other.id] != -1 || capacity[now][other.id] <= 0)
                continue;
            parent[other.id] = now;
            int new_flow = min(flow, capacity[now][other.id]);
            if (other.id == sink) return new_flow;  // reached the sink
            q.push({other.id, new_flow});
        }
    }

    return 0;
}

int maxFlow() {  // O(VE^2)
    int flow = 0;
    vector<int> parent(numberOfNodes + 1);

    while (true) {
        int ret = bfs(parent, s.id, t.id);
        if (ret == 0) break;
        flow += ret;

        int now = t.id;  // start from sink, follow the path towards the source
        while (now != 0) {
            int prev = parent[now];
            capacity[prev][now] -= ret;
            capacity[now][prev] += ret;
            now = prev;
        }
    }

    return flow;
}

void dfs(int node) {
    visited[node] = true;
    for (auto& other : adj[node]) {
        if (!visited[other.id] && capacity[node][other.id] > 0) dfs(other.id);
    }
}

void reportOnATeam(int y) {
    cout << Teams[y].name << " is eliminated.\n";
    cout << "They can win at most " << Teams[y].win << " + " << Teams[y].left
         << " = " << Teams[y].win + Teams[y].left << " games.\n";

    vector<int> toppers;
    // first do some normal checking
    if (maxPossibleWinningID != y &&
        maxPossibleWin > Teams[y].win + Teams[y].left)
        toppers.push_back(maxPossibleWinningID);

    // normal checking not enough
    else {
        dfs(s.id);
        for (int j = 0; j < numberOfTeams; j++)
            if (!visited[Teams[j].nodeID]) toppers.push_back(j);
    }

    const int sz = toppers.size();
    int total_win = 0, total_left = 0, total_between = 0;
    for (int i = 0; i < sz; i++) {
        cout << Teams[toppers[i]].name;
        total_win += Teams[toppers[i]].win;
        total_left += Teams[toppers[i]].left;
        if (i < sz - 2)
            cout << ", ";
        else if (i == sz - 1)
            cout << " and ";
        else
            cout << "\n";
    }
    total_between /= 2;

    cout << (sz == 1 ? "has" : "have") << " won a total of " << total_win
         << " games.\n";
}

int main() {
    cin >> numberOfTeams;
    Teams.resize(numberOfTeams + 1);
    mAgainst.assign(numberOfTeams + 1, vector<int>(numberOfTeams + 1, 0));
    teamPairID.assign(numberOfTeams + 1, vector<int>(numberOfTeams + 1));

    for (int i = 0; i < numberOfTeams; i++) {
        cin >> Teams[i].name >> Teams[i].win >> Teams[i].loss >> Teams[i].left;
        if (Teams[i].win + Teams[i].left > maxPossibleWin) {
            maxPossibleWin = Teams[i].win + Teams[i].left;
            maxPossibleWinningID = i;
        }
        for (int j = 0; j < numberOfTeams; j++) {
            cin >> mAgainst[i][j];
            if (i < j) total_matches += mAgainst[i][j];
        }
    }

    generateNodeIDs();
    buildGraph();

    // cerr << "Number of Nodes: " << numberOfNodes << "\n";

    for (int i = 0; i < numberOfTeams; i++) {
        // now anaylzing for team i
        setCapacity(i);
        int ret = maxFlow();
        // cerr << i << " " << Teams[i].name << " " << ret << "\n";
        if (ret < total_matches) reportOnATeam(i);
    }
}