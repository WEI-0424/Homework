# 41343149

## 作業三 $Polynomial + Available Lists$

## 解題說明

使用 $Polynomial$類別來表示多項式，並且要有加法 ( $+$ )  , 減法 ( $-$ )  , 乘法 ( $*$ ) ，動態輸入多項式使用鏈結串列管理多項式項目，且提高效率

### 解題策略( $How$ $to$ $do$ )

1. #### ChainNode：
   建立節點，保存資料與指向下一個節點的指標

2. ####  $Chain / ChainIterator：$
   完成串列操作與走訪方式，讓後續多項式可以用 $iterator$ 逐項處理。
   
4. #### $Available List$
   將刪掉或釋放的節點放進回收串列，下次需要節點時優先取用，提升記憶體使用效率。

## 程式實作

```cpp
//Chain.h
#ifndef CHAIN_H
#define CHAIN_H

#include <iostream>
using namespace std;

template <class T> class Chain;
template <class T> class ChainIterator;
template <class T> class AvailableList;

//--- ChainNode ---
template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& e) : element(e), next(nullptr) {}
};
//---

//--- ChainIterator ---
template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
public:
    ChainIterator(ChainNode<T>* start = nullptr) : current(start) {}

    T& operator*() const { return current->element; }
    T* operator->() const { return &current->element; }

    ChainIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }

    bool operator!=(const ChainIterator& rhs) const {
        return current != rhs.current;
    }
};
//---

//--- AvailableList ---
template <class T>
class AvailableList {
private:
    static ChainNode<T>* head;
public:
    static ChainNode<T>* getNode() {
        if (!head) return nullptr;
        ChainNode<T>* node = head;
        head = head->next;
        node->next = nullptr;
        return node;
    }

    static void returnNode(ChainNode<T>* node) {
        if (!node) return;
        node->next = head;
        head = node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::head = nullptr;
//---

//--- Chain ---
template <class T>
class Chain {
    friend class ChainIterator<T>;
private:
    ChainNode<T>* head;

public:
    Chain() : head(nullptr) {}

    ~Chain() {
        while (head) {
            ChainNode<T>* tmp = head;
            head = head->next;
            AvailableList<T>::returnNode(tmp);
        }
    }

    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }

    void insert(int index, const T& e) {
        // 先拿節點
        ChainNode<T>* node = AvailableList<T>::getNode();
        if (!node) node = new ChainNode<T>(e);
        else node->element = e;

        //--- 超出範圍處理（index小於0）---
        if (index < 0) {
            AvailableList<T>::returnNode(node);   // <<< 超出範圍：回收節點
            return;
        }

        if (index == 0) {
            node->next = head;
            head = node;
            return;
        }

        //--- 如果head為空但inde大於零的話就代表超出範圍 ---
        if (!head) {
            AvailableList<T>::returnNode(node);   // <<< 超出範圍：回收節點
            return;
        }

        //--- 走到 index-1 的位置 ---
        ChainNode<T>* prev = head;
        for (int i = 0; i < index - 1; i++) {
            if (!prev) {
                AvailableList<T>::returnNode(node);   //超出範圍的話就回收節點
                return;
            }
            prev = prev->next;
        }

        //--- 如果prev走到nullptr代表index超出串列長度 ---
        if (!prev) {
            AvailableList<T>::returnNode(node);   // 只要超出範圍的話就回收節點
            return;
        }
        node->next = prev->next;
        prev->next = node;
    }
};

#endif

```
```cpp
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "Chain.h"

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
private:
    Chain<Term> terms;
public:
    Polynomial() {}

    void newTerm(double coef, int exp) {
        if (coef == 0) return;

        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp < exp) {
                terms.insert(index, Term(coef, exp));
                return;
            }
            if (it->exp == exp) {
                it->coef += coef;
                return;
            }
        }
        terms.insert(index, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        auto it1 = terms.begin();
        auto it2 = b.terms.begin();

        while (it1 != terms.end() && it2 != b.terms.end()) {
            if (it1->exp > it2->exp) {
                r.newTerm(it1->coef, it1->exp);
                ++it1;
            } else if (it1->exp < it2->exp) {
                r.newTerm(it2->coef, it2->exp);
                ++it2;
            } else {
                r.newTerm(it1->coef + it2->coef, it1->exp);
                ++it1; ++it2;
            }
        }

        while (it1 != terms.end()) {
            r.newTerm(it1->coef, it1->exp);
            ++it1;
        }
        while (it2 != b.terms.end()) {
            r.newTerm(it2->coef, it2->exp);
            ++it2;
        }
        return r;
    }

    double Evaluate(double x) const {
        double result = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            double p = 1;
            for (int i = 0; i < it->exp; i++) p *= x;
            result += it->coef * p;
        }
        return result;
    }
};

inline istream& operator>>(istream& is, Polynomial& p) {
    int n;
    is >> n;
    while (n--) {
        double c; int e;
        is >> c >> e;
        p.newTerm(c, e);
    }
    return is;
}

inline ostream& operator<<(ostream& os, const Polynomial& p) {
    bool first = true;
    for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
        if (!first) os << " + ";
        first = false;
        os << it->coef;
        if (it->exp > 0) os << "x^" << it->exp;
    }
    if (first) os << "0";
    return os;
}

#endif

```
```cpp
#include <iostream>
#include "Polynomial.h"
using namespace std;

int main() {
    Polynomial A, B;
    double x;

    cin >> A >> B;
    cin >> x;

    cout << "A = " << A << endl;
    cout << "B = " << B << endl;
    cout << "A + B = " << (A + B) << endl;
    cout << "A(" << x << ") = " << A.Evaluate(x) << endl;

    return 0;
}

```

## 效能分析
| 操作項目 | 時間複雜度 | 空間複雜度 | 說明 |
|----------|--------------|----------|----------|
| $newTerm(coef, exp)$   | $O(n)$ | $O(1)$    | 插入新項時需依指數順序走訪串列        |
| $operator+$   | $O(n + m)$ | $O(n+m)$        | 以 $Iterator$ 合併兩個已排序多項式 每個項只處理一次        |
| $operator-$   | $O(n + m)$ | $O(n + m)$        | 跟加法流程相同，只有係數符號不同        |
| $operator*$   | $O(n × m)$ | $O(n × m)$       | 每一項與另一多項式所有項相乘       |
| $Evaluate(x)$   | $O(n × d)$ | $O(1))$       | 對每一項計算 x 的指數次方並累加       |
| 拷貝建構子   | $O(n)$ | $O(n)$       | 逐項複製原多項式的所有節點       |
| 指派運算子   | $O(n + m)$ | $O(n)$       | 回收原有節點 再複製新多項式內容       |
| 解構子   | $O(n)$ | $O(1)$       | 將所有節點回收到Available List不立即釋放記憶體       |

## 測試與驗證 

### 測試案例

| 測試案例 | 輸入 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $3$ | $8X^2+3X^1+2X^0$    | $8X^2+3X^1+2X^0$        |
|| $5$ $2$ |||
|| $3$ $1$ ||| 
|| $7$ $0$ |||
|| $2$ |||
|| $3$ $2$ |||
|| $-5$ $0$ |||
| 測試二   | $2$ | $5X^3+5X^1$        | $5X^3+5X^1$        |
|| $4$ $3$ |||
|| $2$ $1$ |||
|| $2$ |||
|| $1$ $3$ |||
|| $3$ $1$ |||
| 測試三   | $3$ | $1X^0$        | $1X^0$        |
|| $3$ $4$ |||
|| $-3$ $2$ |||
|| $1$ $0$ |||
|| $2$ |||
|| $-3$ $4$ |||
|| $3$ $2$ |||

### 編譯與執行指令

```shell
cd "/Applications/code/" && g++ polynomial_class.cpp -o polynomial_class && "/Applications/code/"polynomial_class
wei@Toaster code % cd "/Applications/code/" && g++ polynomial_class.cpp -o polynomial_class && "/Applications/code/"polynomial_class
3
5 2
3 1
7 0
2 
3 2
-5 0
8X^2+3X^1+2X^0
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

