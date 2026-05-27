# 41343141
# 41343149


## 解題說明

這一次的作業是要我們實作混合排序 $(Composite$ $Sorting$ $Function)$ 它會根據不同的資料大小而使用不同的排序方式像是:
 
小資料 → 用 $Insertion$ $Sort$

中型資料 → 用 $Quick$ $Sort$

大資料 → 用 $Merge$ / $Heap$

並且找出哪一個排去法在 $Worst$ $case$ 最快，且寫出一個自動選最佳排序法的程式。

所以我們要先實作出四種排序方法
1. $Insertion$ $Sort$
2. $Quick$ $Sort$
3. $Merge$ $Sort$
4. $Heap$ $Sort$
   
然後針對不同的資料量 $n$ 量測每個排序法在 $Worst-Case$ 還有 $average-case$ 下的執行時間，之後比較哪一種排序法在不同情況下表現最佳，最後根據結果設計 $Composite$ $sort$ 使程式可以依照資料量選擇比較適合的排序方法。
   
### 解題策略

1. $Insertion$ $Sort$

   $Insertion$ $Sort$很適合小型資料的排序，而它的優點是程式很簡單還有它的額外空間很少，但是 $Worst$ $Case$ 是 $O(n^2)$ 
   在資料量很小的時候反而會比複雜的排序法還快

2. $Quick$ $Sort$

   $Quick$ $Sort$ 的平均效率很好， 它是 $O(nlogn)$
   這次的題目要求使用 $median-of-three$ 選 $pivot$

3. $Merge$ $Sort$

   $Merge$ $Sort$ 不管是在 $best$ $、$ $average$ $、$ $worst$ $case$ 都是 $O(n*log*n)$

   所以它在　$worst$ $case$　的準則下很穩定

4. $Heap$ $Sort$

   $Heap$ $Sort$ 也是 $worst$ $case$ $:$ $O(n*log*n)$

   而且只需要 $O(1)$ 額外空間，所以也適合 $worst$ $case$ 比較
 
6. $Composite$ $Sort$

   因為這次作業強調的是 $worst-time$ 準則所以要避免在最壞情況下使用 $Quick$ $Sort$ 作為主要選擇，而其他的資料就要 $:$

   小資料量 $:$ 使用 $Insertion$ $Sort$

   大資料量 $:$ 使用 $Heap$ $Sort$ 或是 $Merge$ $Sort$

---

## 程式實作

以下為主要程式碼：

```cpp 
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <ctime>

using namespace std;
using namespace chrono;

// ==================== Insertion Sort ====================
void insertionSort(vector<int>& a) {
    int n = a.size();

    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;

        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }

        a[j + 1] = key;
    }
}

// ==================== Quick Sort ====================

// Partition Function
int partition(vector<int>& a, int left, int right) {

    // median-of-three
    int mid = (left + right) / 2;

    if (a[left] > a[mid])
        swap(a[left], a[mid]);

    if (a[left] > a[right])
        swap(a[left], a[right]);

    if (a[mid] > a[right])
        swap(a[mid], a[right]);

    swap(a[mid], a[right - 1]);

    int pivot = a[right - 1];

    int i = left;
    int j = right - 1;

    while (true) {

        while (a[++i] < pivot);

        while (a[--j] > pivot);

        if (i >= j)
            break;

        swap(a[i], a[j]);
    }

    swap(a[i], a[right - 1]);

    return i;
}

// Quick Sort Recursive
void quickSort(vector<int>& a, int left, int right) {

    if (left + 10 <= right) {

        int pivotIndex = partition(a, left, right);

        quickSort(a, left, pivotIndex - 1);
        quickSort(a, pivotIndex + 1, right);
    }
    else {

        // 小區間改用 Insertion Sort
        for (int i = left + 1; i <= right; i++) {

            int key = a[i];
            int j = i - 1;

            while (j >= left && a[j] > key) {
                a[j + 1] = a[j];
                j--;
            }

            a[j + 1] = key;
        }
    }
}

// ==================== Merge Sort ====================

// Merge Function
void merge(vector<int>& a, vector<int>& temp,
           int left, int mid, int right) {

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {

        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid)
        temp[k++] = a[i++];

    while (j <= right)
        temp[k++] = a[j++];

    for (int x = left; x <= right; x++)
        a[x] = temp[x];
}

// Merge Sort Recursive
void mergeSort(vector<int>& a, vector<int>& temp,
               int left, int right) {

    if (left < right) {

        int mid = (left + right) / 2;

        mergeSort(a, temp, left, mid);
        mergeSort(a, temp, mid + 1, right);

        merge(a, temp, left, mid, right);
    }
}

// Wrapper
void mergeSortWrapper(vector<int>& a) {

    vector<int> temp(a.size());

    mergeSort(a, temp, 0, a.size() - 1);
}

// ==================== Heap Sort ====================

// Heapify
void heapify(vector<int>& a, int n, int i) {

    int largest = i;

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest])
        largest = left;

    if (right < n && a[right] > a[largest])
        largest = right;

    if (largest != i) {

        swap(a[i], a[largest]);

        heapify(a, n, largest);
    }
}

// Heap Sort
void heapSort(vector<int>& a) {

    int n = a.size();

    // Build Heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i);

    // Extract Elements
    for (int i = n - 1; i > 0; i--) {

        swap(a[0], a[i]);

        heapify(a, i, 0);
    }
}

// ==================== Measure Time ====================

template <typename SortFunc>
double measureTime(vector<int> a, SortFunc sortFunc) {

    auto start = high_resolution_clock::now();

    sortFunc(a);

    auto end = high_resolution_clock::now();

    duration<double, milli> elapsed = end - start;

    return elapsed.count();
}

// ==================== Generate Worst Case ====================

vector<int> generateWorstCase(int n) {

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }

    return a;
}

// ==================== Main ====================

int main() {

    srand(time(0));

    vector<int> testSize = {
        500,
        1000,
        2000,
        3000,
        4000,
        5000
    };


    for (int n : testSize) {

        cout << "n = " << n << endl;

        vector<int> data = generateWorstCase(n);

        // Heap / Quick 使用 random permutation
        shuffle(data.begin(),
                data.end(),
                default_random_engine(time(0)));

        // Insertion Sort
        double tInsertion =
            measureTime(data, insertionSort);

        // Quick Sort
        double tQuick =
            measureTime(data, [](vector<int>& a) {
                quickSort(a, 0, a.size() - 1);
            });

        // Merge Sort
        double tMerge =
            measureTime(data, mergeSortWrapper);

        // Heap Sort
        double tHeap =
            measureTime(data, heapSort);

        cout << "Insertion Sort : "
             << tInsertion << " ms\n";

        cout << "Quick Sort     : "
             << tQuick << " ms\n";

        cout << "Merge Sort     : "
             << tMerge << " ms\n";

        cout << "Heap Sort      : "
             << tHeap << " ms\n";

        cout << "-----------------------------\n";
    }

    return 0;
}
```

---

## 效能分析

### (1) $LinkedGraph$
| 演算法 | 時間複雜度                 | 空間複雜度           |
| ---- | -------------------- | -------------- | 
| $DFS$  | $O$ $(V+E)$           | $O$ $(V)$    | 
| $BFS$  | $O$ $(V+E)$           | $O$ $(V)$    | 
| $Connected$ $Components$  | $O$ $(V+E)$           | $O$ $(V)$    | 

### (2) $WeightGraph$
#### $Prim$
| 方法 | 時間複雜度                 | 
| ---- | -------------------- | 
| $Prim$  | $O$ $(V^2)$           | 

#### $Kruskal$
| 方法 | 時間複雜度                 | 
| ---- | -------------------- | 
| $Kruskal$  | $O$ $(E log E)$           | 
#### $Dijkstra$
| 方法 | 時間複雜度                 | 
| ---- | -------------------- | 
| $Dijkstra$  | $O$ $(E log V)$           | 

#### 空間複雜度
| 結構 | 空間                 | 
| ---- | -------------------- | 
| $adjacency$ $list$ | $O$ $(V + E))$           | 
| $dist / visited$  | $O$ $(V)$           | 

## 測試與驗證

### 測試案例

#### $DFS$

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | $DFS$ $from$ $0$           | $0$ $1$ $3$ $4$ $2$    | $0$ $1$ $3$ $4$ $2$   |
| 測試二  | $DFS$ $from$ $1$           | $1$ $0$ $2$ $3$ $4$    | $1$ $0$ $2$ $3$ $4$    |
| 測試三  | $DFS$ $from$ $5$          | $5$ | $5$ |

#### $BFS$

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | $BFS$ $from$ $0$           | $0$ $1$ $2$ $3$ $4$    | $0$ $1$ $2$ $3$ $4$   |
| 測試二  | $BFS$ $from$ $1$           | $1$ $0$ $3$ $2$ $4$    | $1$ $0$ $3$ $2$ $4$    |
| 測試三  | $BFS$ $from$ $5$          | $5$ | $5$ |

#### $Connected$ $Components$

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | 原始圖           | $2$ 個 $Components$ | $2$ 個 $Components$   |
| 測試二  | Components$ $1$           | $0$ $1$ $3$ $2$ $4$    | $0$ $1$ $3$ $2$ $4$    |
| 測試三  | $Components$ $2$          | $5$ | $5$ |

#### $Prim$

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | 原始圖           | $2$ 個 $Components$ | $2$ 個 $Components$   |
| 測試二  | $Components$ $1$           | $0$ $1$ $3$ $2$ $4$    | $0$ $1$ $3$ $2$ $4$    |
| 測試三  | $Components$ $2$          | $5$ | $5$ |

#### $Kruskal$

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | 原始圖           | $Total$ $Cost:19$ | $Total$ $Cost:19$   |
| 測試二  | $MST$ 邊           | $(2-3,4),$ $(0-3,5),$ $(0-1,10)$    | $(2-3,4),$ $(0-3,5),$ $(0-1,10)$    |
| 測試三  | 邊數   | $3$ 條邊 | $3$ 條邊 |

#### $Dijkstra$

| 測試案例 | 輸入參數                 | 預期輸出           | 實際輸出           |
| ---- | -------------------- | -------------- | -------------- |
| 測試一  | $start=$ $0$           | $0→0$ $=$ $0$ | $0→0$ $=$ $0$   |
| 測試二  | $start=$ $0$           | $0→1$ $=$ $10$    | $0→1$ $=$ $10$    |
| 測試三  | $start=$ $0$   | 0→2$ $=$ $6$ | 0→2$ $=$ $6$ |
| 測試四  | $start=$ $0$   | $0→3$ $=$ $5$ | $0→3$ $=$ $5$ |

### 程式輸出結果

```shell 
n = 500
Insertion Sort : 0.504875 ms
Quick Sort     : 0.064 ms
Merge Sort     : 0.132959 ms
Heap Sort      : 0.138209 ms
-----------------------------
n = 1000
Insertion Sort : 1.894 ms
Quick Sort     : 0.140334 ms
Merge Sort     : 0.286541 ms
Heap Sort      : 0.305583 ms
-----------------------------
n = 2000
Insertion Sort : 6.79921 ms
Quick Sort     : 0.252666 ms
Merge Sort     : 0.493041 ms
Heap Sort      : 0.485458 ms
-----------------------------
n = 3000
Insertion Sort : 11.8921 ms
Quick Sort     : 0.297666 ms
Merge Sort     : 0.594458 ms
Heap Sort      : 0.658041 ms
-----------------------------
n = 4000
Insertion Sort : 17 ms
Quick Sort     : 0.343209 ms
Merge Sort     : 0.700958 ms
Heap Sort      : 0.735042 ms
-----------------------------
n = 5000
Insertion Sort : 22.2105 ms
Quick Sort     : 0.382875 ms
Merge Sort     : 0.745833 ms
Heap Sort      : 0.7935 ms
-----------------------------
```

### 編譯與執行指令

```shell 
wei@Mac code % g++ -std=c++17 Sorting_3.cpp -o Sorting_3 && ./Sorting_3
```

---

## 結論

1. 本題使用 $Adjacency$ $List$ 來表示圖形，程式寫起來比較好理解。
2. $Adjacency$ $List$ 比較適合邊數沒有很多的圖，因為不用存不存在的邊。
3. $DFS$ 是一直往深處走，適合用來看圖形能不能走到某個點。
4. $BFS$ 是一層一層搜尋，適合用來找比較近的節點。
5. $Kruskal$ 可以找出最小生成樹，也就是用最小成本把所有點連起來。
6. $Dijkstra$ 可以從一個起點出發，算出到其他點的最短距離。
7. 加權圖和無加權圖差在有沒有 $weight$，如果有 $weight$，就可以做最短路徑和最小生成樹。

---

## 為甚麼用 $Adjacency$ $List$ 來解題

1. **比較省空間**
   如果用 $Adjacency$ $Matrix$，要開一個二維陣列，就算兩個點沒有連線，也還是會佔位置。
   但是 $Adjacency$ $List$ 只記錄真的有連線的邊，所以比較不浪費空間。

2. **比較適合這次的程式**
   這次要做 $DFS$ 、 $BFS$ 、 $Dijkstra$ ，這些演算法都會一直找某個點旁邊連到誰，所以用 $Adjacency$ $List$ 比較方便。

3. **加權圖比較好寫**
   因為每條邊都有權重，所以我用 `pair<int, int>` 來存資料。第一個 $int$ 是連到哪個點，第二個 $int$ 是權重。

4. **我比較容易理解**
   對我來說，$Adjacency$ $List$ 比較像是每個點都有一張自己的朋友清單，裡面記錄它連到誰、距離多少，所以理解起來比較直覺。

---

## 加權圖與無加權圖差別

無加權圖只需要知道兩個點有沒有連接，所以可以這樣寫：

```cpp 
vector<vector<int>> graph;
```

加權圖除了知道有沒有連接，還要記錄邊的權重，所以這次用：

```cpp 
vector<vector<pair<int, int>>> graph;
```

其中：

```cpp
pair<int, int>
```

第一個 $int$ 是相鄰頂點，第二個 $int$ 是權重。

例如：

```cpp 
adjList[0].push_back(make_pair(1, 4));
```

意思是：

```text 
0 連到 1，這條邊的權重是 4
```

所以如果只是單純走訪圖，可以用無加權圖；但如果要算最短路徑或最小成本，就需要用加權圖。

---
## 申論及開發報告

本程式主要以 $Graph$ 圖形資料結構為核心，實作圖形的建立、走訪、最小生成樹與最短路徑演算法。程式使用 $Adjacency$ $List$ 作為圖形表示法，將每一個頂點所連接的相鄰頂點與邊的權重存入串列中。此方式適合用於邊數較少的圖形，能有效降低不必要的空間浪費。

在圖形建立部分，程式透過 $addEdge()$ 函式加入邊。由於本題使用的是無向加權圖，所以每加入一條邊時，會同時將 $u$ 到 $v$ 以及 $v$ 到 $u$ 的資料加入 $Adjacency$ $List$ 中。此外，程式也會將邊的資料另外存入 $edges$ 陣列，方便後續 $Kruskal$ $Algorithm$ 進行排序與選邊。

在圖形走訪部分，程式分別實作 $DFS$ 與 $BFS$ 。 $DFS$ 使用遞迴方式進行深度優先搜尋，從起點開始，遇到尚未走訪的相鄰頂點就繼續往下搜尋。$BFS$ 則使用 $queue$ 進行廣度優先搜尋，先走訪距離起點較近的頂點，再逐層往外擴展。兩者都會使用 $visited$ 陣列來記錄頂點是否已經走訪，避免重複拜訪同一個頂點。

在最小生成樹部分，程式使用 $Kruskal$ $Algorithm$ 。此演算法會先將所有邊依照權重由小到大排序，再依序選擇權重較小的邊加入生成樹。為了避免產生 $cycle$ ，程式使用 $DSU$ 結構進行判斷。 $find()$ 函式用來找出頂點所屬集合， $unite()$ 函式則用來合併兩個不同集合。當兩個頂點已經在同一集合中時，代表加入該邊會形成 $cycle$ ，因此不會選取該邊。

在最短路徑部分，程式使用 $Dijkstra$ $Algorithm$ 。此演算法從指定起點開始，使用 $dist$ 陣列記錄目前已知的最短距離，並搭配 $riority$ $queue$ 取得目前距離最小的頂點。當找到更短的路徑時，就更新該頂點的距離，直到所有可到達頂點的最短距離都被計算完成。

本程式的測試流程是先建立一個包含 $6$ 個頂點的無向加權圖，再依序執行 $DFS$ 、 $BFS$ 、 $Kruskal$ $Algorithm$ 和 $Dijkstra$ $Algorithm$ 。 $DFS$ 與 $BFS$ 用來確認圖形連線與走訪是否正常； $Kruskal$ $Algorithm$ 用來確認是否能找出正確的最小生成樹總成本； $Dijkstra$ $Algorithm$ 則用來確認從起點到其他頂點的最短距離是否正確。

整體而言，本程式完成了圖形資料結構的基本建立與多種圖形演算法實作，包含圖形走訪、最小生成樹與最短路徑計算，能夠展示 $Graph$ 在不同問題中的基本應用方式。

---

## 心得與討論

這次作業讓我對 $Graph$ 有比較基本的理解。原本我只比較熟悉陣列、 $stack$ 、 $queue$ 或 $tree $， $Graph$ 對我來說比較難，因為每個頂點可以連到很多不同的頂點，不像樹有固定的父子關係，所以一開始會覺得比較混亂。

透過這次實作，我知道 $Graph$ 可以用不同方式儲存，而我這次使用 $Adjacency$ $List$ ，是因為它比較省空間，也比較容易看出每個頂點連到哪些地方。雖然程式裡面有些地方還不是很熟，例如 $DSU$ 和 $priority$ $queue$ ，但至少我知道它們在 $Kruskal$ 和 $Dijkstra$ 裡面是很重要的工具。

整體來說，這次作業對我來說不算簡單，但透過一步一步實作，我比較了解圖形資料結構的基本想法，也知道不同演算法有不同用途。這次最大的收穫是讓我知道 $Graph$ 不只是課本上的東西，它也可以用在路線、網路連線或很多實際問題上。

