# 41343141
# 41343149


## 解題說明

本題主要是參考 Graph 簡報內容，來做一個圖形資料結構的程式。簡報裡有提到 Graph ADT、圖形表示法、Adjacency Matrix、Adjacency List、Weighted Edges、DFS、BFS、Minimum Cost Spanning Trees 和 Shortest Paths 等內容，所以我這次是用 Adjacency List 來實作圖形，並且在程式裡面加入 DFS、BFS、Kruskal 最小生成樹，以及 Dijkstra 最短路徑。

一開始我對 Graph 的概念其實沒有很熟，因為它不像一般陣列或串列那麼直覺。Graph 是由很多點和邊組成，每個點可以連到其他點，所以我先用比較容易理解的方式，把每個點旁邊連到誰都記錄下來。

### 解題策略

1. 先了解 Graph 是由頂點 Vertex 和邊 Edge 組成。
2. 使用 Adjacency List 來表示圖，因為這種方式比較省空間。
3. 如果是無加權圖，只要記錄相鄰頂點就可以。
4. 如果是加權圖，除了記錄相鄰頂點，還要記錄邊的權重。
5. 用 DFS 來做深度搜尋。
6. 用 BFS 來做廣度搜尋。
7. 用 Kruskal 找出最小生成樹。
8. 用 Dijkstra 找出從起點到其他點的最短距離。

---

## 程式實作

以下為主要程式碼：

```cpp id="85gcc4"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

class Edge {
public:
    int u;
    int v;
    int w;

    Edge(int a, int b, int c) {
        u = a;
        v = b;
        w = c;
    }
};

class DSU {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB) {
            return false;
        }

        if (rank[rootA] < rank[rootB]) {
            parent[rootA] = rootB;
        }
        else if (rank[rootA] > rank[rootB]) {
            parent[rootB] = rootA;
        }
        else {
            parent[rootB] = rootA;
            rank[rootA]++;
        }

        return true;
    }
};

class Graph {
private:
    int vertex;
    vector<vector<pair<int, int>>> adjList;
    vector<Edge> edges;

    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (int i = 0; i < adjList[v].size(); i++) {
            int next = adjList[v][i].first;

            if (!visited[next]) {
                DFSUtil(next, visited);
            }
        }
    }

public:
    Graph(int v) {
        vertex = v;
        adjList.resize(vertex);
    }

    void addEdge(int u, int v, int w) {
        adjList[u].push_back(make_pair(v, w));
        adjList[v].push_back(make_pair(u, w));
        edges.push_back(Edge(u, v, w));
    }

    void DFS(int start) {
        vector<bool> visited(vertex, false);

        cout << "DFS: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    void BFS(int start) {
        vector<bool> visited(vertex, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS: ";

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << current << " ";

            for (int i = 0; i < adjList[current].size(); i++) {
                int next = adjList[current][i].first;

                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        cout << endl;
    }

    void dijkstra(int start) {
        vector<int> dist(vertex, INT_MAX);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int currentDist = pq.top().first;
            int current = pq.top().second;
            pq.pop();

            if (currentDist > dist[current]) {
                continue;
            }

            for (int i = 0; i < adjList[current].size(); i++) {
                int next = adjList[current][i].first;
                int weight = adjList[current][i].second;

                if (dist[current] + weight < dist[next]) {
                    dist[next] = dist[current] + weight;
                    pq.push(make_pair(dist[next], next));
                }
            }
        }

        cout << "Dijkstra Shortest Path from " << start << ":" << endl;

        for (int i = 0; i < vertex; i++) {
            cout << start << " to " << i << " = " << dist[i] << endl;
        }
    }

    void kruskal() {
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
            return a.w < b.w;
        });

        DSU dsu(vertex);
        int totalCost = 0;

        cout << "Kruskal MST:" << endl;

        for (int i = 0; i < edges.size(); i++) {
            int u = edges[i].u;
            int v = edges[i].v;
            int w = edges[i].w;

            if (dsu.unite(u, v)) {
                cout << u << " - " << v << " : " << w << endl;
                totalCost += w;
            }
        }

        cout << "Total Cost: " << totalCost << endl;
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 5);
    g.addEdge(2, 3, 8);
    g.addEdge(2, 4, 10);
    g.addEdge(3, 4, 2);
    g.addEdge(3, 5, 6);
    g.addEdge(4, 5, 3);

    g.DFS(0);
    g.BFS(0);

    cout << endl;

    g.kruskal();

    cout << endl;

    g.dijkstra(0);

    return 0;
}
```

---

## 效能分析

1. Adjacency List 的空間複雜度是 *O(V + E)*。
   V 是頂點數，E 是邊數。因為它只會存真的有連接的邊，所以比 Adjacency Matrix 省空間。

2. DFS 的時間複雜度是 *O(V + E)*。
   因為每個頂點最多會被走訪一次，每條邊也大概只會檢查一次。

3. BFS 的時間複雜度也是 *O(V + E)*。
   BFS 用 queue 來做，會一層一層往外搜尋。

4. Kruskal 的時間複雜度是 *O(E log E)*。
   因為它要先把所有邊依照權重排序，排序通常是最花時間的地方。

5. Dijkstra 的時間複雜度大約是 *O((V + E) log V)*。
   因為我用 priority queue 來找目前距離最小的點。

6. 整體空間複雜度大約是 *O(V + E)*。
   因為程式裡面需要存 adjacency list、visited、distance 和 edge list。

---

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | DFS from 0           | 0 1 2 3 4 5    | 0 1 2 3 4 5    |
| 測試二  | BFS from 0           | 0 1 2 3 4 5    | 0 1 2 3 4 5    |
| 測試三  | Kruskal MST          | Total Cost: 13 | Total Cost: 13 |
| 測試四  | Dijkstra from 0 to 5 | 13             | 13             |

### 程式輸出結果

```shell id="stxfzt"
DFS: 0 1 2 3 4 5
BFS: 0 1 2 3 4 5

Kruskal MST:
1 - 2 : 1
0 - 2 : 2
3 - 4 : 2
4 - 5 : 3
1 - 3 : 5
Total Cost: 13

Dijkstra Shortest Path from 0:
0 to 0 = 0
0 to 1 = 3
0 to 2 = 2
0 to 3 = 8
0 to 4 = 10
0 to 5 = 13
```

### 編譯與執行指令

```shell id="uuek7z"
g++ graph.cpp -o graph.exe
graph.exe
```

---

## 結論

1. 本題使用 Adjacency List 來表示圖形，程式寫起來比較好理解。
2. Adjacency List 比較適合邊數沒有很多的圖，因為不用存不存在的邊。
3. DFS 是一直往深處走，適合用來看圖形能不能走到某個點。
4. BFS 是一層一層搜尋，適合用來找比較近的節點。
5. Kruskal 可以找出最小生成樹，也就是用最小成本把所有點連起來。
6. Dijkstra 可以從一個起點出發，算出到其他點的最短距離。
7. 加權圖和無加權圖差在有沒有 weight，如果有 weight，就可以做最短路徑和最小生成樹。

---

## 為甚麼用 Adjacency List 來解題

1. **比較省空間**
   如果用 Adjacency Matrix，要開一個二維陣列，就算兩個點沒有連線，也還是會佔位置。
   但是 Adjacency List 只記錄真的有連線的邊，所以比較不浪費空間。

2. **比較適合這次的程式**
   這次要做 DFS、BFS、Dijkstra，這些演算法都會一直找某個點旁邊連到誰，所以用 Adjacency List 比較方便。

3. **加權圖比較好寫**
   因為每條邊都有權重，所以我用 `pair<int, int>` 來存資料。第一個 int 是連到哪個點，第二個 int 是權重。

4. **我比較容易理解**
   對我來說，Adjacency List 比較像是每個點都有一張自己的朋友清單，裡面記錄它連到誰、距離多少，所以理解起來比較直覺。

---

## 加權圖與無加權圖差別

無加權圖只需要知道兩個點有沒有連接，所以可以這樣寫：

```cpp id="bcvz3h"
vector<vector<int>> graph;
```

加權圖除了知道有沒有連接，還要記錄邊的權重，所以這次用：

```cpp id="g4gkmp"
vector<vector<pair<int, int>>> graph;
```

其中：

```cpp id="qoshgp"
pair<int, int>
```

第一個 int 是相鄰頂點，第二個 int 是權重。

例如：

```cpp id="dve2h9"
adjList[0].push_back(make_pair(1, 4));
```

意思是：

```text id="yz5wui"
0 連到 1，這條邊的權重是 4
```

所以如果只是單純走訪圖，可以用無加權圖；但如果要算最短路徑或最小成本，就需要用加權圖。

---

## 心得與討論

這次作業對我來說有一點難，因為我本來對 Graph 的概念沒有很熟。以前比較常看到的是陣列、stack、queue 或 tree，Graph 則是每個點可以連到很多點，一開始會覺得有點亂，不太知道資料要怎麼存。

後來我用 Adjacency List 的方式去理解，感覺比較清楚。每一個頂點都有自己的清單，清單裡面放它連到的其他頂點，還有邊的權重。這樣想之後，Graph 就比較不像一堆看不懂的連線，而是比較像每個點都有自己的鄰居資料。

DFS 和 BFS 也是我這次比較有收穫的地方。DFS 是一直往深的地方走，走到底再回來；BFS 是先把旁邊的點走完，再慢慢往外擴散。雖然兩個都是走訪圖，但做法不太一樣。

Kruskal 和 Dijkstra 對我來說比較難，尤其是 Kruskal 裡面的 DSU，我一開始不太懂為什麼要用它。後來才知道它是用來判斷兩個點是不是已經連在一起，避免最小生成樹產生 cycle。Dijkstra 則是用來算從一個起點到其他點的最短距離，這個比較像地圖導航，所以比較容易想像。

總結來說，這次作業讓我對 Graph 有比較基本的理解。雖然程式裡面有些地方還需要再多練習，例如 DSU 和 priority queue，但至少我知道圖形可以怎麼存、怎麼走訪，也知道加權圖可以拿來做最短路徑和最小生成樹。對我這種程式能力還不太好的人來說，這次算是有慢慢理解 Graph 的基本概念。
