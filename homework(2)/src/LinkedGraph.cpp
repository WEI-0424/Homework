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
