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
        adj[u].push_back({ v, w });
        adj[v].push_back({ u, w });
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
                    edges.push_back({ w, u, v });
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
        pq.push({ 0, start });

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (auto [v, w] : adj[u]) {
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.push({ dist[v], v });
                }
            }
        }

        cout << "Dijkstra Shortest Paths from " << start << ":\n";

        for (int i = 0; i < n; i++) {
            cout << start << " -> " << i << " = ";

            if (dist[i] == INT_MAX) {
                cout << "INF";
            }
            else {
                cout << dist[i];
            }

            cout << endl;
        }
    }
};
