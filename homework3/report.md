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
#ifndef CHAIN_H
#define CHAIN_H

#include <iostream>
using namespace std;

template<class T> class Chain;
template<class T> class ChainIter;
template<class T> class Avail;

//--- ChainNode ---
template<class T>
class Node {
    friend class Chain<T>;
    friend class ChainIter<T>;
    friend class Avail<T>;
private:
    T data;
    Node* next;
public:
    Node(): next(nullptr) {}
    Node(const T& x): data(x), next(nullptr) {}
};
//--- end ChainNode ---


//--- AvailableList ---
template<class T>
class Avail {
private:
    static Node<T>* pool;
public:
    static Node<T>* take() {
        if (!pool) return nullptr;
        Node<T>* p = pool;
        pool = pool->next;
        p->next = nullptr;
        return p;
    }

    static void give(Node<T>* first) {
        if (!first) return;
        Node<T>* last = first;
        while (last->next) last = last->next;
        last->next = pool;
        pool = first;
    }

    static Node<T>* make(const T& x) {
        Node<T>* p = take();
        if (!p) p = new Node<T>(x);
        else p->data = x;
        return p;
    }
};

template<class T>
Node<T>* Avail<T>::pool = nullptr;
//--- end AvailableList ---


//--- ChainIterator ---
template<class T>
class ChainIter {
private:
    Node<T>* cur;
public:
    ChainIter(Node<T>* p = nullptr): cur(p) {}

    T& operator*() const { return cur->data; }
    T* operator->() const { return &(cur->data); }

    ChainIter& operator++() {
        if (cur) cur = cur->next;
        return *this;
    }

    bool operator!=(const ChainIter& rhs) const { return cur != rhs.cur; }
    bool operator==(const ChainIter& rhs) const { return cur == rhs.cur; }
};
//--- end ChainIterator ---


//--- Chain ---
template<class T>
class Chain {
private:
    Node<T>* head;
public:
    Chain(): head(nullptr) {}

    Chain(const Chain& other): head(nullptr) {
        int idx = 0;
        for (ChainIter<T> it = other.begin(); it != other.end(); ++it, ++idx)
            insert(idx, *it);
    }

    Chain& operator=(const Chain& other) {
        if (this == &other) return *this;
        clear();
        int idx = 0;
        for (ChainIter<T> it = other.begin(); it != other.end(); ++it, ++idx)
            insert(idx, *it);
        return *this;
    }

    ~Chain() { clear(); }

    void clear() {
        Avail<T>::give(release());
    }

    Node<T>* release() {
        Node<T>* old = head;
        head = nullptr;
        return old;
    }

    ChainIter<T> begin() const { return ChainIter<T>(head); }
    ChainIter<T> end()   const { return ChainIter<T>(nullptr); }

    void insert(int idx, const T& x) {
        Node<T>* n = Avail<T>::make(x);

        if (idx <= 0 || !head) {
            n->next = head;
            head = n;
            return;
        }

        Node<T>* prev = head;
        for (int i = 0; i < idx - 1 && prev; ++i)
            prev = prev->next;

        if (!prev) {
            Avail<T>::give(n);
            return;
        }

        n->next = prev->next;
        prev->next = n;
    }
};
//--- end Chain ---

#endif


```
```cpp
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "Chain.h"
#include <iostream>
using namespace std;


//--- Term ---
struct Term {
    double c;
    int e;
    Term(): c(0), e(0) {}
    Term(double coef, int exp): c(coef), e(exp) {}
};
//--- end Term ---


//--- Polynomial ---
class Polynomial {
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);

private:
    Chain<Term> seq;

public:
    Polynomial() {}

    Polynomial(const Polynomial& other): seq(other.seq) {}

    Polynomial& operator=(const Polynomial& other) {
        seq = other.seq;
        return *this;
    }

    ~Polynomial() {}

    ChainIter<Term> begin() const { return seq.begin(); }
    ChainIter<Term> end()   const { return seq.end(); }

    void addTerm(double coef, int exp) {
        if (coef == 0) return;

        if (begin() == end()) {
            seq.insert(0, Term(coef, exp));
            return;
        }

        int idx = 0;
        for (ChainIter<Term> it = begin(); it != end(); ++it, ++idx) {
            if (it->e == exp) {
                it->c += coef;
                return;
            }
            if (it->e < exp) {
                seq.insert(idx, Term(coef, exp));
                return;
            }
        }
        seq.insert(idx, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& rhs) const {
        Polynomial result;
        ChainIter<Term> i = begin();
        ChainIter<Term> j = rhs.begin();

        while (i != end() && j != rhs.end()) {
            if (i->e > j->e) {
                result.addTerm(i->c, i->e);
                ++i;
            }
            else if (i->e < j->e) {
                result.addTerm(j->c, j->e);
                ++j;
            }
            else {
                double s = i->c + j->c;
                if (s != 0) result.addTerm(s, i->e);
                ++i;
                ++j;
            }
        }

        while (i != end()) {
            result.addTerm(i->c, i->e);
            ++i;
        }

        while (j != rhs.end()) {
            result.addTerm(j->c, j->e);
            ++j;
        }

        return result;
    }

    Polynomial operator-(const Polynomial& rhs) const {
        Polynomial neg;
        for (ChainIter<Term> it = rhs.begin(); it != rhs.end(); ++it)
            neg.addTerm(-it->c, it->e);
        return (*this) + neg;
    }

    Polynomial operator*(const Polynomial& rhs) const {
        Polynomial result;
        for (ChainIter<Term> i = begin(); i != end(); ++i)
            for (ChainIter<Term> j = rhs.begin(); j != rhs.end(); ++j)
                result.addTerm(i->c * j->c, i->e + j->e);
        return result;
    }

    double eval(double x) const {
        double ans = 0;
        for (ChainIter<Term> it = begin(); it != end(); ++it) {
            double p = 1;
            for (int k = 0; k < it->e; ++k) p *= x;
            ans += it->c * p;
        }
        return ans;
    }
};
//--- end Polynomial ---


//--- input operator ---
inline istream& operator>>(istream& is, Polynomial& p) {
    int n;
    if (!(is >> n)) return is;
    for (int i = 0; i < n; ++i) {
        double c;
        int e;
        is >> c >> e;
        p.addTerm(c, e);
    }
    return is;
}
//--- end input operator ---


//--- output operator ---
inline ostream& operator<<(ostream& os, const Polynomial& p) {
    bool first = true;
    for (ChainIter<Term> it = p.begin(); it != p.end(); ++it) {
        if (!first) os << "+";
        first = false;

        if (it->e == 0)
            os << it->c;
        else
            os << it->c << "x^" << it->e;
    }
    if (first) os << 0;
    return os;
}
//--- end output operator ---

#endif

```
```cpp
// main.cpp
int main() {
    Polynomial A, B;
    double x;

    cin >> A >> B >> x;

    cout << "A = " << A << "\n";
    cout << "B = " << B << "\n";
    cout << "A + B = " << (A + B) << "\n";
    cout << "A - B = " << (A - B) << "\n";
    cout << "A * B = " << (A * B) << "\n";
    cout << "A(" << x << ") = " << A.eval(x) << "\n";
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

