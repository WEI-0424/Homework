#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
#include <climits>
#include <functional>

using namespace std;

/* ================= LinkedGraph ================= */
class LinkedGraph {
private:
    int n;
    vector<vector<int>> adj;

    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (int u : adj[v]) {
            if (!visited[u]) {
                DFSUtil(u, visited);
            }
        }
    }

public:
    LinkedGraph(int n) : n(n) {
        adj.resize(n);
    }

    void insertEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void display() {
        cout << "Adjacency List:\n";
        for (int i = 0; i < n; i++) {
            cout << i << ": ";
            for (int j : adj[i]) {
                cout << j << " ";
            }
            cout << endl;
        }
    }

    void DFS(int start) {
        vector<bool> visited(n, false);
        cout << "DFS: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    void BFS(int start) {
        vector<bool> visited(n, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS: ";
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            cout << v << " ";

            for (int u : adj[v]) {
                if (!visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }
            }
        }
        cout << endl;
    }

    void ConnectedComponents() {
        vector<bool> visited(n, false);
        int count = 0;

        cout << "Connected Components:\n";

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                cout << "Component " << count + 1 << ": ";
                DFSUtil(i, visited);
                cout << endl;
                count++;
            }
        }

        cout << "Total Components: " << count << endl;
    }
};

/* ================= WeightedGraph ================= */
class WeightedGraph {
private:
    int n;
    vector<vector<pair<int, int>>> adj;

public:
    WeightedGraph(int n) : n(n) {
        adj.resize(n);
    }

    void insertEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    void display() {
        cout << "Weighted Adjacency List:\n";
        for (int i = 0; i < n; i++) {
            cout << i << ": ";
            for (auto [v, w] : adj[i]) {
                cout << "(" << v << ", " << w << ") ";
            }
            cout << endl;
        }
    }

    void Prim(int start) {
        vector<int> key(n, INT_MAX);
        vector<int> parent(n, -1);
        vector<bool> inMST(n, false);

        key[start] = 0;
        int totalCost = 0;

        for (int i = 0; i < n; i++) {
            int u = -1;

            for (int j = 0; j < n; j++) {
                if (!inMST[j] && (u == -1 || key[j] < key[u])) {
                    u = j;
                }
            }

            if (u == -1) break;

            inMST[u] = true;
            totalCost += key[u];

            for (auto [v, w] : adj[u]) {
                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                }
            }
        }

        cout << "Prim MST:\n";
        for (int i = 0; i < n; i++) {
            if (parent[i] != -1) {
                cout << parent[i] << " - " << i << " : " << key[i] << endl;
            }
        }

        cout << "Prim MST Total Cost: " << totalCost << endl;
    }

    void Kruskal() {
        vector<tuple<int, int, int>> edges;

        for (int u = 0; u < n; u++) {
            for (auto [v, w] : adj[u]) {
                if (u < v) {
                    edges.push_back({w, u, v});
                }
            }
        }

        sort(edges.begin(), edges.end());

        vector<int> parent(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        function<int(int)> find = [&](int x) {
            if (parent[x] == x) return x;
            return parent[x] = find(parent[x]);
        };

        int totalCost = 0;

        cout << "Kruskal MST:\n";

        for (auto [w, u, v] : edges) {
            int rootU = find(u);
            int rootV = find(v);

            if (rootU != rootV) {
                parent[rootU] = rootV;
                totalCost += w;

                cout << u << " - " << v << " : " << w << endl;
            }
        }

        cout << "Kruskal MST Total Cost: " << totalCost << endl;
    }

    void Dijkstra(int start) {
        vector<int> dist(n, INT_MAX);
        dist[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (auto [v, w] : adj[u]) {
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Dijkstra Shortest Paths from " << start << ":\n";

        for (int i = 0; i < n; i++) {
            cout << start << " -> " << i << " = ";

            if (dist[i] == INT_MAX) {
                cout << "INF";
            } else {
                cout << dist[i];
            }

            cout << endl;
        }
    }
};

/* ================= main ================= */
int main() {
    cout << "=== LinkedGraph ===\n";

    LinkedGraph lg(6); // 頂點 5 是孤立點
    lg.insertEdge(0, 1);
    lg.insertEdge(0, 2);
    lg.insertEdge(1, 3);
    lg.insertEdge(3, 4);

    lg.display();
    lg.DFS(0);
    lg.BFS(0);
    lg.ConnectedComponents();

    cout << "\n=== WeightedGraph ===\n";

    WeightedGraph wg(4);
    wg.insertEdge(0, 1, 10);
    wg.insertEdge(0, 2, 6);
    wg.insertEdge(0, 3, 5);
    wg.insertEdge(1, 3, 15);
    wg.insertEdge(2, 3, 4);

    wg.display();
    cout << endl;

    wg.Prim(0);
    cout << endl;

    wg.Kruskal();
    cout << endl;

    wg.Dijkstra(0);

    return 0;
}
