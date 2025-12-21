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
| 多項式A項數   | $3$ | -    | -        |
|多項式A| $3$ $4$ |-|-|
|多項式A| $2$ $2$ |-|-| 
|多項式A| $1$ $0$ |-|-|
|多項式B項數| $2$ |-|-|
|多項式B| $5$ $3$ |-|-|
|多項式B| $4$ $1$ |-|-|
|X值| $2$ |-|-|
### 運算結果:
| 測試案例 | 輸入 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
|A| - | $3x⁴ + 2x² + 1$ | $3x⁴ + 2x² + 1$ |
|B| - | $5x³ + 4x$ | $B = 5x³ + 4x$ |
|A+B| - | $3x^4+5x^3+2x^2+4x+1$ | $3x^4+5x^3+2x^2+4x+1$ |
|A-B| - | $3x^4-5x^3+2x^2-4x+1$ | $3x^4-5x^3+2x^2-4x+1$ |
|A*B| - | $15x^7+12x^5+5x^3+8x^3+4x$ | $15x^7+12x^5+5x^3+8x^3+4x$ |
|A(2)| - |57|57|
### 編譯與執行指令

```shell
cd "/Applications/code/" && g++ polynomial_homework3.cpp -o  && polynomial_homework3&&"/Applications/code/"polynomial_homework3
wei@Mac code % cd "/Applications/code/" && g++ polynomial_homework3.cpp -o polynomial_homework3 && "/Applications/code/"polynomial_homework3
3
3 4
2 2
1 0
2
5 3
4 1
2
A = 3x^4 + 2x^2 + 1
B = 5x^3 + 4x^1
A + B = 3x^4 + 5x^3 + 2x^2 + 4x^1 + 1
A - B = 3x^4 + -5x^3 + 2x^2 + -4x^1 + 1
A * B = 15x^7 + 22x^5 + 13x^3 + 4x^1
A(2) = 57
```
## 申論及開發報告

### 申論
$Term結構表示多項式得項目，而且內涵係數(coef)還有指數(exp)表示最基本的多項式元素$
$而在Term之上則是使用ChainNode來做為鏈結串列的節點單位，將每一個Term包裝成可串接的節點，也可以不受固定大小限制，可以彈性新增或是合併項目$
$整個串列都是Chain類別在管理 關於節點的插入、走訪跟釋放。還有搭配ChainIterator節點存取，可以讓多項式的運算用逐項比較完成$
$還有使用Available List將用不到的節點回收跟重複利用$
#### 結論與心得討論
這次的作業不像是以往的使用原本的main.cpp一個檔案，使用原本就有的標頭檔。而是要將它們都切開來實作，因為語法上還有使用方式上有較多差異所以我在撰寫的時候發生了許多錯誤導致要慢慢除錯，也讓我了解到我的程式撰寫方面還有許多的進步空間，也讓我學到許多以前都沒有碰過可以讓程式運行更有效率的小技巧，但是在將這次的作業寫完後我發現我對於程式碼的理解有變強許多，總而言之我認為這次的程式作業對我來說算是一個小挑戰也讓我知道我有哪些地方更需要補足以及加強。

