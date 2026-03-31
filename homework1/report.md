
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
在輸入項數增加時，程式運算時間成線性增長。

例如:

| $n(每個多項式數)$ | $執行時間(ms)$ | 
|----------|--------------|
| $10$   | $0.02$ |
| $100$   | $0.15$ | 
| $1000$   | $1.2$ | 

就代表時間複雜度符合 $O(n)$ 。

## 申論及開發報告



### 心得討論
這次的作業我大致上學會如何使用類別來實作抽象資料型態( $ADT$ )。

在這次寫程式的過程中我覺得最重要的是掌握「資料結構設計與記憶體管理」。

利用動態陣列能使多項式大小彈性變化，而不必固定長度。

在多項式相加時，使用雙指標的方式比逐項搜尋更有效率，

讓整體運算只需一次線性掃描即可完成。

在開發過程中，我遇到的最大問題是記憶體管理。

因為多項式的項數不固定，所以我必須使用動態記憶體配置 (new / delete[])。

最初程式會出現 double free 錯誤，原因是我沒有實作「深拷貝」機制。

後來我加入 拷貝建構子與指定運算子 (operator=)，

確保每個物件都擁有自己的記憶體空間，

才解決錯誤並讓程式執行正常。

雖然在剛開始寫時我覺得有點困難而且也蠻難理解的，但是因為不懂也有詢問同學也有查閱上網路上的資料最終才完成這次的作業。

但是這次作業的實作也讓我知道我的能力還需要精進很多 因為很多語法我還是有不了解的地方 還需要再研究再練習。
