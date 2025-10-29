# 41343149

## 作業一 $Polynomial$ $Class$

## 解題說明

這一次的作業要求實作一個 Polynomial類別，要以程式方式建立多項式，並且支援基本運算與輸入輸出功能。

而多項式由多個「項」組成，每項包含：一個係數 ( $coefficient$ )、一個指數 ( $exponent$ )

就像是:

$A(x)=5X³+2X²-7X+1$

可以表示成:
| $係數$ | $指數$ | 
|----------|--------------|
| $5$   | $3$ |
| $2$   | $2$ | 
| $-7$   | $1$ | 
| $1$   | $0$ |

為了要落實多項式，所以我們以類別設計( $ADT$ )實作

### 解題策略( $How$ $to$ $do$ )

1. #### 資料結構設計
   使用類別設計法（ $ADT$ ）
   
   $Term$ ：用來儲存一項的係數與指數。
   
   而 $Polynomial$ :包含一個Term陣列與項目數
2. #### 多項式加法演算法
   使用兩個指標 $aPos$、 $bPos$ 逐一比較兩個多項式的項：

   若指數相同 → 係數相加後插入結果。

   若一邊指數較大 → 優先插入該項。
3. #### 動態記憶體管理
   初始容量 2。若項目超出容量，自動擴充成兩倍。

## 程式實作

```cpp
/*
Algorithm Design:
1. 使用兩個類別 Term 與 Polynomial 來表示多項式。
2. newTerm(): 將輸入的係數與指數加入 termArray，如果容量不夠就擴充成兩倍。
3. Add(): 使用雙指標走訪兩個多項式，若指數相同則係數相加。
4. operator>>(): 輸入項數與 (coef, exp)。
5. operator<<(): 依序輸出每一項，格式為 coefX^exp。
*/
#include <iostream>
using namespace std;

class Polynomial;

//用來儲存一項的係數與指數
class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream &output, const Polynomial &Poly);
private:
    int exp;// 指數
    float coef;// 係數
};

// Polynomial 類別：用動態陣列表示多項式
class Polynomial {
private:
    Term *termArray; // 指向 Term 陣列
    int capacity; // 陣列容量
    int terms; // 目前多項式的項數
public:
   // 建構子：初始化容量與項數
    Polynomial(): capacity(2), terms(0) {
        termArray = new Term[capacity];
    }
 // 解構子：釋放動態記憶體
    ~Polynomial() {
        delete [] termArray;
    }
 // 拷貝建構子：避免淺拷貝造成重複釋放
    Polynomial(const Polynomial& other): capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }
// 指派運算子：實作深拷貝
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        Term* newArr = new Term[other.capacity];
        for (int i = 0; i < other.terms; ++i) newArr[i] = other.termArray[i];
        delete[] termArray;
        termArray = newArr;
        capacity = other.capacity;
        terms = other.terms;
        return *this;
    }
// 多項式加法
    Polynomial Add(const Polynomial& b) const;
// 新增新項
    void newTerm(const float newcoef, const int newexp);
// 輸入與輸出運算子
    friend istream& operator>>(istream& is, Polynomial& poly);
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
};
// operator>>：輸入
istream& operator>>(istream& is, Polynomial& poly) {
    float coef;
    int exp, n;
    is >> n;
    while (n--) {
        is >> coef >> exp;
        poly.newTerm(coef, exp);
    }
    return is;
}
// operator<<：輸出
ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; ++i) {
        if (i > 0) os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}
// Add()：兩個多項式相加
Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial r;
    int i = 0, j = 0;
    while (i < terms && j < b.terms) {
        if (termArray[i].exp == b.termArray[j].exp) {
            float s = termArray[i].coef + b.termArray[j].coef;
            if (s) r.newTerm(s, termArray[i].exp);
            ++i; ++j;
        } else if (termArray[i].exp < b.termArray[j].exp) {
            r.newTerm(b.termArray[j].coef, b.termArray[j].exp);
            ++j;
        } else {
            r.newTerm(termArray[i].coef, termArray[i].exp);
            ++i;
        }
    }
    while (i < terms) { r.newTerm(termArray[i].coef, termArray[i].exp); ++i; }
    while (j < b.terms) { r.newTerm(b.termArray[j].coef, b.termArray[j].exp); ++j; }
    return r;
}
// newTerm()：在多項式中新增一個項目
void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; ++i) temp[i] = termArray[i];
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms].exp = theExp;
    ++terms;
}

int main() {
    Polynomial a, b, c;
    cin >> a >> b;
    c = a.Add(b);
    cout << c << endl;// 輸出結果
    return 0;
}

}
```


## 效能分析
| 函式 | 功能 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|----------|
| $newTerm()$   | 新增新項 | $O(1)$、擴容時 $O(n)$    | $O(n)$        |
| $Add()$   | 多項式加法 | $O(n+m)$        | $O(max(n,m))$        |
| $operator>>$   | 輸入 | $O(n)$        | $O(n)$        |
| $operator<<$   | 輸出 | $O(n)$       | $O(1)$       |

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

雖然在剛開始寫時我覺得有點困難而且野蠻難理解的，但是因為不懂也有詢問同學也有查閱上網路上的資料最終才完成這次的作業。

但是這次作業的實作也讓我知道我的能力還需要精進很多 因為很多語法我還是有不了解的地方 還需要再研究再練習。
