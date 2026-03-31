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

