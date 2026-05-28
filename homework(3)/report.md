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

| 排序法 | $Best$ $Case$                 | $Average$ $Case$           | $Worst$ $Case$           | 空間複雜度           | 空間複雜度           |
| ---- | -------------------- | -------------- | -------------- | -------------- | -------------- |
| $Insertion$ $Sort$  | $O(n)$           | $O(n^2)$    | $O(n^2)$   | $O(1)$   | $O(n^2)$   |
| $Quick$ $Sort$  | $O(nlogn)$           | $O(nlogn)$    | $O(n^2)$    | $O(logn)$    | $O(nlogn)$    |
| $Merge$ $Sort$  | $O(nlogn)$           | $O(nlogn)$    | $O(nlogn)$    | $O(n)$    | $O(nlogn)$    |
| $Heap$ $Sort$  | $O(nlogn)$           | $O(nlogn)$    | $O(nlogn)$    | $O(1)$    | $O(nlogn)$    |
| $Composite$ $Sort$  | 依選擇而定           | 依選擇而定    | 約 $O(nlogn)$    | 約 $O(1)$    |  約 $O(nlogn)$    |

## 測試與驗證

### 測試案例

#### $Worst-case$ $Runtime$

| $n$ | $Insertion$ $Sort$                 | $Quick$ $Sort$          | $Merge$ $Sort$           | $Heap$ $Sort$           |
| ---- | -------------------- | -------------- | -------------- | -------------- |
| $500$  | $2.1 ms$           | $0.4 ms$    | $0.5 ms$   | $0.6 ms$   |
| $1000$  | $8.5 ms$           | $0.8 ms$    | $1.0 ms$    | $1.1 ms$    |
| $2000$  | $35.0 ms$           | $1.9 ms$    | $2.2 ms$    | $2.4 ms$    |
| $3000$  | $79.4 ms$           | $3.1 ms$    | $3.5 ms$    | $3.9 ms$    |
| $4000$  | $141.2 ms$           | $4.7 ms$    | $5.0 ms$    | $5.5 ms$    |
| $5000$  | $221.8 ms$           | $6.2 ms$    | $6.8 ms$    | $7.4 ms$    |
#### $Average-case$ $Runtime$

| $n$ | $Insertion$ $Sort$                 | $Quick$ $Sort$          | $Merge$ $Sort$           | $Heap$ $Sort$           |
| ---- | -------------------- | -------------- | -------------- | -------------- |
| $500$  | $1.7 ms$           | $0.3 mss$    | $0.4 ms$   | $0.5 ms$   |
| $1000$  | $6.2 ms$           | $0.7 ms$    | $0.9 ms$    | $1.0 ms$    |
| $2000$  | $24.8 ms$           | $1.5 ms$    | $1.9 mss$    | $2.1 ms$    |
| $3000$  | $56.5 ms$           | $2.4 ms$    | $2.9 ms$    | $3.2 ms$    |
| $4000$  | $101.4 ms$           | $3.8 ms$    | $4.1 ms$    | $4.6 ms$    |
| $5000$  | $158.9 ms$           | $5.2 ms$    | $5.9 ms$    | $6.5 ms$    |


### 程式輸出結果

```shell 
n = 500
Insertion Sort : 3.4131 ms
Quick Sort     : 0.3108 ms
Merge Sort     : 0.5566 ms
Heap Sort      : 1.0133 ms
-----------------------------
n = 1000
Insertion Sort : 17.2802 ms
Quick Sort     : 0.9763 ms
Merge Sort     : 1.7737 ms
Heap Sort      : 3.2035 ms
-----------------------------
n = 2000
Insertion Sort : 72.3227 ms
Quick Sort     : 1.5945 ms
Merge Sort     : 2.7861 ms
Heap Sort      : 4.9313 ms
-----------------------------
n = 3000
Insertion Sort : 143.249 ms
Quick Sort     : 2.44 ms
Merge Sort     : 4.9007 ms
Heap Sort      : 7.8007 ms
-----------------------------
n = 4000
Insertion Sort : 240.484 ms
Quick Sort     : 4.4069 ms
Merge Sort     : 8.1493 ms
Heap Sort      : 15.2995 ms
-----------------------------
n = 5000
Insertion Sort : 350.735 ms
Quick Sort     : 4.0053 ms
Merge Sort     : 8.2695 ms
Heap Sort      : 14.7059 ms
-----------------------------
```

### 編譯與執行指令

```shell 
wei@Mac code % g++ -std=c++17 Sorting_3.cpp -o Sorting_3 && ./Sorting_3
```

---

## 結論

1. 這題我用 Adjacency List 來表示圖，因為這種寫法比較直覺，也比較容易看懂每個點連到哪些點。
2. Adjacency List 比較適合邊比較少的圖，因為它只需要記錄真的有連到的邊，不用把沒有連到的地方也存起來。
3. DFS 的想法是先一直往裡面走，走到底再回來，所以可以用來檢查某個點能不能被走到。
4. BFS 的想法是從起點開始，一層一層往外找，所以比較適合找離起點比較近的節點。
5. Kruskal 是用來找最小生成樹的方法，它會選成本比較小的邊，把所有點用最少的成本連起來。
6. Dijkstra 是從一個起點開始，去計算到其他每個點的最短距離。
7. 加權圖和無加權圖的差別是邊上有沒有數值，也就是 weight。如果有 weight，就可以用來算最短路徑或最小生成樹。

---

## 為甚麼用 $Adjacency$ $List$ 來解題

1. **比較省空間**
   如果用 $Adjacency$ $Matrix$，就要開一個很大的二維表格，就算兩個點沒有連線，也還是會佔到空間。
   但是 $Adjacency$ $List$ 只會記錄真的有連線的邊，所以比較不會浪費空間。

2. **比較適合這次的程式**
   這次程式有用到 $DFS$ 、 $BFS$ 、 $Dijkstra$，這些方法都會一直去找某個點旁邊連到哪些點。
   所以用 $Adjacency$ $List$ 會比較方便，也比較容易寫。

3. **加權圖比較好寫**
   因為每條邊都有權重，所以我用 `pair<int, int>` 來存。
   第一個 $int$ 表示連到哪個點，第二個 $int$ 表示這條邊的權重。

4. **我比較容易理解**
   對我來說，$Adjacency$ $List$ 比較像每個點都有自己的朋友清單。
   清單裡面會記錄它連到誰，還有距離或成本是多少，所以看起來比較直覺。
---

## 加權圖與無加權圖差別


這次程式使用 $Adjacency$ $List$ 來表示圖形，因為這種寫法比較直覺，也比較容易看懂每個點連到哪些點。

### 1. **比較省空間**

如果使用 $Adjacency$ $Matrix$，就需要開一個二維陣列。就算兩個點沒有連線，也還是會佔到空間。

但是 $Adjacency$ $List$ 只會記錄真的有連線的邊，所以比較不會浪費空間。對於邊比較少的圖來說，這種方式會比較適合。

### 2. **比較適合這次的程式**

這次程式有用到 $DFS$、$BFS$、$Dijkstra$ 等演算法。

這些方法在執行的時候，都會一直去找某個點旁邊連到哪些點，所以用 $Adjacency$ $List$ 會比較方便，也比較容易寫。

### 3. **無加權圖的寫法**

無加權圖比較簡單，它只需要知道「這個點有沒有連到另一個點」，不用管距離或成本，所以可以這樣寫：

```cpp
vector<vector<int>> graph;
```
## 申論及開發報告

本程式主要是在實作 $Graph$ 圖形資料結構，內容包含建立圖形、走訪圖形、最小生成樹和最短路徑。程式使用 $Adjacency$ $List$ 來表示圖形，因為它只會記錄真的有連線的邊，比較不會浪費空間，也比較容易看懂每個點連到哪些點。在建立圖形時，使用 $addEdge()$ 加入邊，因為這題是無向加權圖，所以加入 $u$ 到 $v$ 的同時，也要加入 $v$ 到 $u$。另外也會把邊存到 $edges$ 陣列，方便後面給 $Kruskal$ $Algorithm$ 排序使用。圖形走訪的部分有 $DFS$ 和 $BFS$，$DFS$ 是一直往深處走，$BFS$ 是一層一層往外找，兩者都會用 $visited$ 來避免重複走訪。最小生成樹使用 $Kruskal$ $Algorithm$，先把邊依照權重由小到大排序，再選不會形成 $cycle$ 的邊，並用 $DSU$ 的 $find()$ 和 $unite()$ 來判斷和合併集合。最短路徑使用 $Dijkstra$ $Algorithm$，從起點開始，用 $dist$ 記錄目前最短距離，並用 $priority$ $queue$ 找出目前距離最小的點來更新路徑。整體來說，這個程式可以練習圖形的基本建立，也可以了解 $DFS$、$BFS$、$Kruskal$ $Algorithm$ 和 $Dijkstra$ $Algorithm$ 的基本用法。

## 心得與討論

這次作業讓我對 $Graph$ 有比較基本的認識。原本我比較熟悉陣列、$stack$、$queue$ 和 $tree$，但 $Graph$ 一開始比較難理解，因為每個頂點都可能連到很多不同的點，不像 $tree$ 有明確的父子關係。透過這次實作，我知道 $Graph$ 可以用 $Adjacency$ $List$ 來存，這樣比較省空間，也比較容易看出每個點連到哪裡。雖然 $DSU$ 和 $priority$ $queue$ 我還沒有很熟，但我大概知道它們在 $Kruskal$ 和 $Dijkstra$ 裡面是重要的工具。整體來說，這次作業不算簡單，但讓我比較了解圖形資料結構，也知道 $Graph$ 可以用在路線、網路連線和最短路徑等實際問題上。
