
# 41343149

## 作業一 $Binary$ $Search$ $Tree$

## 解題說明

這次的作業要要我們實作一個最小優先佇列的抽象類別 $MinPQ$ ，並以 $MinHeap$ 類別繼承它，後完成最小堆積的基本操作。
$MinPQ$ 與 $MinHeap$ 最小堆積是一種完全二元樹結構，並滿足每個父節點的值都小於或等於子節點，因此根節點永遠是整個堆積中的最小值。透過這種性質，可以有效率地完成插入、取得最小值與刪除最小值等操作。

### 解題策略( $How$ $to$ $do$ )

1. #### 抽象類別設計
   要先定義一個 $MinPQ$ 抽象類別，包含：
   • $IsEmpty()$ 判斷是否為空
   • $Top()$ 取得最小值
   • $Push()$ 插入元素
   • $POP()$ 刪除最小值
2. #### 最小堆積結構
   使用動態陣列來表示 $heap$ ，根節點放在 $heap[1]$ ，如此可利用：
	•	父節點：i/2
	•	左子節點：2*i
	•	右子節點：2*i+1
3. #### 插入演算法
   新元素放在後面，再用上濾方式與父節點比較，如果比較小就往上移動直到與 $min$ $heap$ 性質符合
4. #### 刪除演算法
   再刪除跟節點後，把最後一個元素補到根之後再用下濾的方式往下調整，才能保持 $min$ $heap$的結構
5. #### 動態記憶體管理
   當陣列的空間不夠時，就會自動擴充兩倍來避免容量空間不夠的問題

## 程式實作

```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

// ===== 抽象類別 MinPQ =====
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

// ===== MinHeap 類別宣告 =====
template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int currentSize;

    void ChangeSize();

public:
    MinHeap(int theCapacity = 10);
    ~MinHeap();

    bool IsEmpty() const override;
    const T& Top() const override;
    void Push(const T& x) override;
    void Pop() override;

    void PrintHeap() const;
};

// ===== ChangeSize：擴充陣列大小 =====
template <class T>
void MinHeap<T>::ChangeSize() {
    capacity *= 2;
    T* newHeap = new T[capacity];
    for (int i = 1; i <= currentSize; i++) {
        newHeap[i] = heap[i];
    }
    delete[] heap;
    heap = newHeap;
}

// ===== 建構子與解構子 =====
template <class T>
MinHeap<T>::MinHeap(int theCapacity) {
    capacity = theCapacity;
    currentSize = 0;
    heap = new T[capacity];
}

template <class T>
MinHeap<T>::~MinHeap() {
    delete[] heap;
}

// ===== IsEmpty：判斷是否為空 =====
template <class T>
bool MinHeap<T>::IsEmpty() const {
    return currentSize == 0;
}

// ===== Top：回傳最小值 =====
template <class T>
const T& MinHeap<T>::Top() const {
    if (IsEmpty()) throw runtime_error("MinHeap is empty");
    return heap[1];
}

// ===== Push：插入元素 =====
template <class T>
void MinHeap<T>::Push(const T& x) {
    if (currentSize == capacity - 1) {
        ChangeSize();
    }

    int i = ++currentSize;
    while (i != 1 && x < heap[i / 2]) {
        heap[i] = heap[i / 2];
        i /= 2;
    }
    heap[i] = x;
}

// ===== Pop：刪除最小值 =====
template <class T>
void MinHeap<T>::Pop() {
    if (IsEmpty()) throw runtime_error("MinHeap is empty");

    T last = heap[currentSize--];
    int i = 1;
    int child = 2;

    while (child <= currentSize) {
        if (child < currentSize && heap[child + 1] < heap[child]) {
            child++;
        }

        if (last <= heap[child]) break;

        heap[i] = heap[child];
        i = child;
        child *= 2;
    }

    heap[i] = last;
}

// ===== PrintHeap：輸出目前 heap =====
template <class T>
void MinHeap<T>::PrintHeap() const {
    for (int i = 1; i <= currentSize; i++) {
        cout << heap[i] << " ";
    }
    cout << endl;
}

// ===== 主程式 main =====
int main() {
    MinHeap<int> h;

    h.Push(12);
    h.Push(6);
    h.Push(8);
    h.Push(7);
    h.Push(15);
    h.Push(3);

    cout << "Min Heap: ";
    h.PrintHeap();

    cout << "Top = " << h.Top() << endl;

    h.Pop();
    cout << "After Pop: ";
    h.PrintHeap();

    return 0;
}
```


## 效能分析
| 函式 | 功能 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|----------|
| $IsEmpty()$   | 判斷 $heap$ 是否為空 | $O(1)$    | $O(n)$        |
| $Top()$   | 取得最小元素 | $O(1)$        | $O(1)$        |
| $Push()$   | 插入元素 | $O(log n)$        | $O(1)$        |
| $POP()$   | 刪除最小元素 | $O(log n)$       | $O(1)$       |
| $ChangeSize()$   | 擴充陣列 | $O(n)$       | $O(n)$       |
| $PrintHeap()$   | 輸出 $heap$ 陣列 | $O(n)$       | $O(1)$       |

## 測試與驗證 

### 測試案例

| 測試案例 | 輸入 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $Push$ $12,6,8,7,15,3$ | $3,7,6,12,15,8$    | $3,7,6,12,15,8$        |
| 測試二   | $TOP()$ | $3$        | $3$        |
| 測試三   | $After$ $POP$ | $6,7,8,12,15$        | $6,7,8,12,15$        |
### 編譯與執行指令

```shell
cd "/Applications/code/" && g++ max_min_heap.cpp -o max_min_heap && "/Applications/code/"max_min_heap
wei@Mac code % cd "/Applications/code/" && g++ max_min_heap.cpp -o max_min_heap && "/Applications/code/"max_min_heap
Min Heap: 3 7 6 12 15 8 
Top = 3
After Pop: 6 7 8 12 15
```
## 效能量測
在 $heap$ 中，插入與刪除都只需要沿著樹高調整，因此隨著元素素量增加，時間就會成對數成長

例如:

| $n$ | $Push/POP$ 平均時間 | 
|----------|--------------|
| $10$   | $很短$ |
| $100$   | $稍增$ | 
| $1000$   | $維持對數增長趨勢$ | 

這就代表最小堆積的主要操作符合 $O(log n)$ 的設計 

## 申論及開發報告



### 心得討論
這次的作業讓我更清楚抽象類別與繼承的概念，也讓我知道 $heap$ 不只是二元樹的結構而已，而是一種能有效率處理 $priority$ $queue$ 的資料結構。
在一開始設計 $MinPQ$ 時，我先從介面思考，在想著一個最小優先佇列應該需要具備哪些基本功能，再由 $MinHeap$ 去實作這些函式。這種寫法讓程式架構比較清楚，也比較符合物件導向設計的方式。
並且這次實作時我也再次練習到動態陣列與記憶體管理，尤其在 $ChangeSize()$ 中需要把舊資料搬到新空間，再釋放原本陣列。整體來說，這題讓我對 $heap$ 的運作流程、上濾與下濾調整，以及時間複雜度都有更完整的理解。
但是我覺得經過一個寒假以及過年後，因為都沒有接觸程式，所以對於程式的敏感度下降了，對於程式的問題變得沒有那麼的直覺了

## 作業二 $Binary$ $Search$ $Tree$

## 解題說明

第二個作業是需要我們完成 $Binary$ $Search$ $Tree$ $（BST）$ 的兩個部分：
	1.	建立空的 $BST$ ，插入 $n$ 個隨機數，測量其高度，並計算 $height / log₂n$
	2.	實作刪除指定 $key$ 的函式，並分析其時間複雜度

$BST$ 的特性是：
	•	左子樹所有鍵值都小於根節點
	•	右子樹所有鍵值都大於根節點

這樣可以有效率地進行搜尋、插入與刪除

### 解題策略( $How$ $to$ $do$ )

1. #### 節點結構設計
   使用 $Node$ 結構表示 $BST$ 節點，每個節點包含：
	•	$key$
	•	$left$
	•	$right$
2. #### 隨機插入與高度量測
   從空樹開始，利用亂數產生器產生 $n$ 個數值，逐一插入 $BST$。插入完後，再使用遞迴方式計算高度
3. #### 刪除演算法
   刪除 $key$ 會時分成三種情況：
	•	無子節點：直接刪除
	•	只有一個子節點：由子節點取代
	•	有兩個子節點：找右子樹最小值取代，再刪除該節點
4. #### 遞迴實作
   插入、刪除、搜尋最小值、高度計算都使用遞迴或簡單迭代方式
## 程式實作
$(A)$ 小題程式碼
```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

// ===== BST 節點結構 =====
struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) {
        key = k;
        left = nullptr;
        right = nullptr;
    }
};

// ===== 插入節點到 BST =====
Node* insert(Node* root, int x) {
    if (root == nullptr)
        return new Node(x);

    if (x < root->key)
        root->left = insert(root->left, x);

    else if (x > root->key)
        root->right = insert(root->right, x);

    return root;
}

// ===== 計算 BST 高度 =====
int height(Node* root) {
    if (root == nullptr)
        return 0;

    int lh = height(root->left);
    int rh = height(root->right);

    return 1 + max(lh, rh);
}

// ===== 釋放整棵樹記憶體 =====
void clear(Node* root) {
    if (!root) return;

    clear(root->left);
    clear(root->right);
    delete root;
}

// ===== 主程式 main =====
int main() {
    srand((unsigned)time(nullptr)); // 初始化亂數種子

    int testN[] = {100, 500, 1000, 2000, 3000, 5000, 10000};

    for (int i = 0; i < 7; i++) {
        int n = testN[i];
        Node* root = nullptr;

        for (int j = 0; j < n; j++) {
            int x = rand();
            root = insert(root, x);
        }

        int h = height(root);

        // 計算 height / log2(n)
        double ratio = h / log2((double)n);

        cout << "n = " << n
             << ", height = " << h
             << ", height/log2(n) = " << ratio << endl;

        clear(root);
    }

    return 0;
}
```
$(B)$ 小題程式碼
```cpp
#include <iostream>
using namespace std;

// ===== BST 節點結構 =====
struct Node {
    int key;        // 節點的值
    Node* left;     // 指向左子樹
    Node* right;    // 指向右子樹

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// ===== insert：插入節點到 BST =====
Node* insert(Node* root, int k) {
    if (root == nullptr)
        return new Node(k);

    // 小於根節點 → 插入左子樹
    if (k < root->key)
        root->left = insert(root->left, k);

    // 大於根節點 → 插入右子樹
    else if (k > root->key)
        root->right = insert(root->right, k);

    return root;
}

// ===== findMin：找右子樹最小節點 =====
Node* findMin(Node* root) {
    while (root && root->left != nullptr)
        root = root->left;

    return root;
}

// ===== deleteNode：刪除指定 key 的節點 =====
Node* deleteNode(Node* root, int k) {
    if (root == nullptr)
        return nullptr;

    // 如果 key 比目前節點小 → 往左找
    if (k < root->key) {
        root->left = deleteNode(root->left, k);
    }
    // 如果 key 比目前節點大 → 往右找
    else if (k > root->key) {
        root->right = deleteNode(root->right, k);
    }
    else {
        // 情況 1：沒有子節點
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        // 情況 2：只有右子節點
        else if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        // 情況 3：只有左子節點
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        // 情況 4：有兩個子節點
        else {
            Node* temp = findMin(root->right);

            root->key = temp->key;

            root->right = deleteNode(root->right, temp->key);
        }
    }
    return root;
}

// ===== inorder：中序走訪 BST（由小到大輸出）=====
void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// ===== clearTree：刪除整棵樹（釋放記憶體）=====
void clearTree(Node* root) {
    if (root != nullptr) {
        clearTree(root->left);
        clearTree(root->right);
        delete root;
    }
}

// ===== 主程式 main =====
int main() {
    Node* root = nullptr;
    int n, x, k;

    cout << "請輸入節點數量: ";
    cin >> n;

    cout << "請輸入 " << n << " 個節點值: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        root = insert(root, x);
    }

    cout << "原本 BST 的 inorder: ";
    inorder(root);
    cout << endl;

    cout << "請輸入要刪除的 key: ";
    cin >> k;

    root = deleteNode(root, k);

    cout << "刪除後 BST 的 inorder: ";
    inorder(root);
    cout << endl;

    clearTree(root);

    return 0;
}
```
## 效能分析
# $(A)$ 小題程式
| 函式 | 功能 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|----------|
| $insert()$   | 插入節點 | $O(h)$    | $O(h)$        |
| $height()$   | 計算樹高 | $O(n)$        | $O(h)$        |
| $clear()$   | 刪除整棵樹 | $O(n)$        | $O(h)$        |

在隨機插入情況下：
$h$ ≈ $O(log n)$

因此整體插入 $n$ 個節點時間：
$O(nlog n)$

# $(B)$ 小題程式
| 函式 | 功能 | 時間複雜度 |
|----------|--------------|----------|
| $insert()$   | 插入節點 | $O(h)$    | 
| $findMin()$   | 找出最小節點 | $O(h)$        |
| $deleteNode()$   | 刪除節點 | $O(h)$        |
| $inorder()$   | 中序走訪 | $O(n)$        |

因此 delete 的時間複雜度：

$Time$ $Complexity$ $of$ $Delete$ = $O(h)$
	•	平均情況： $O(log n)$
	•	最壞情況： $O(n)$
## 測試與驗證 

### 測試案例
# $(A)$ 小題程式
| $n$ | $height$ | $height/log2n$ |
|----------|--------------|----------|
| $100$   | $11$ | $1.65$    |
| $500$   | $19$ | $2.11$        |
| $1000$   | $20$ | $2.00$        |
| $2000$   | $27$ | $2.46$        |
| $5000$  | $25$ | $2.03$        |
| $10000$  | $31$ | $2.33$        |

# $(B)$ 小題程式
| 測試案例 | 輸入 $BST$ | 刪除 $key$ | 刪除後 $inorder$ |
|----------|--------------|----------|----------|
| 測試一   | $10$ $7$ $12$ $3$ $11$ $9$ $16$ | $3$    | $7$ $9$ $10$ $11$ $12$ $16$       |
| 測試二   | $10$ $7$ $12$ $3$ $11$ $9$ $16$  | $12$        | $3$ $7$ $9$ $10$ $11$ $16$        |
| 測試三   | $10$ $7$ $12$ $3$ $11$ $9$ $16$ | $10$        | $3$ $7$ $9$ $11$ $12$ $16$        |
### 編譯與執行指令
# $(A)$ 小題程式
```shell
ei@Mac code % cd "/Applications/code/" && g++ binary_search_tree_a.cpp -o binary_search_tree_a && "/Applications/code/"binary_search_tree_a
n = 100, height = 11, height/log2(n) = 1.65566
n = 500, height = 19, height/log2(n) = 2.11917
n = 1000, height = 20, height/log2(n) = 2.00687
n = 2000, height = 27, height/log2(n) = 2.4622
n = 5000, height = 25, height/log2(n) = 2.03455
n = 10000, height = 31, height/log2(n) = 2.33298
```
# $(B)$ 小題程式
```shell
wei@Mac code % cd "/Applications/code/" && g++ binary_search_tree_b.cpp -o binary_search_tree_b && "/Applications/code/"binary_search_tree_b
請輸入節點數量: 10
請輸入 10 個節點值: 10 7 12 3 11 9 16 2 5 14
原本 BST 的 inorder: 2 3 5 7 9 10 11 12 14 16 
請輸入要刪除的 key: 12
刪除後 BST 的 inorder: 2 3 5 7 9 10 11 14 16 
```
## 效能量測
# $(A)$ 小題
在不同的 $n$值下測量BST高度

例如:

| 函式 | 功能 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|----------|
| $insert()$   | 插入一個節點 | $O(h)$    | $O(h)$       |
| $height()$   | 計算樹高  | $O(n)$        | $O(h)$        |
| $clear()$   | 清除整棵樹 | $O(n)$        | $O(h)$        |

# $(B)$ 小題
| 函式 | 功能 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|----------|
| $insert()$   | 插入節點 | $O(h)$    | $O(h)$       |
| $findMin()$   | 找右子樹最小節點  | $O(h)$        | $O(1)$        |
| $deleteNode()$   | 刪除指定節點 | $O(h)$        | $O(h)$        |
| $inorder()$   | 中序走訪輸出  | $O(n)$        | $O(h)$        |
| $clearTree()$   | 刪除整棵樹 | $O(n)$        | $O(h)$        |

## 申論及開發報告



### 心得討論
這次的作業讓我更清楚 $Binary$ $Search$ $Tree$ 的基本操作，尤其是插入、計算高度與刪除三種情況的處理方式。
在刪除節點時，我覺得最重要的是分清楚：沒有子節點、只有一個子節點、有兩個子節點
我覺得最麻煩的是有兩個子節點時，就必須找右子樹最小值來取代，在一開始我超級容易因為這樣而混亂，但在實作後就比較能理解 $BST$ 的維護方式。
並且在做隨機插入並量測高度時，也讓我實際觀察到 $BST$ 的高度並不一定固定，而是會受到插入順序影響。隨機資料下的樹高大多與 $log₂n$ 接近
這兩題讓我同時練習到：樹的遞迴操作、動態記憶體配置、$BST$ 的結構維護、時間複雜度分析
對我來說這些技巧還是有一點困難的，也讓我知道我有哪些部分更需要我努力！
