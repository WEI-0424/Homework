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
cd "/Applications/code/" && g++ ackermann.cpp -o ackermann && "/Applications/code/"ackermann
wei@Mac code % cd "/Applications/code/" && g++ ackermann.cpp -o ackermann && "/Applications/code/"ackermann

請輸入 m 與 n ：2 3
A(2, 3) = 9
請輸入 m 與 n ：3 3
A(3, 3) = 61
請輸入 m 與 n ：4 0
A(4, 0) = 13
```

### 結論

1. $Ackermann$ 函數的遞迴結構清晰、易於理解，但極易導致記憶體堆疊爆炸。  
2. 使用堆疊模擬的非遞迴方法雖然能避免系統遞迴限制，但運算量仍指數級增加。

## 申論及開發報告

### 選擇遞迴方法的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. 程式撰寫簡潔且易於理解  
   遞迴的寫法能用極少的程式碼呈現複雜的運算流程。每個條件分支都對應到特定數學規則，程式可讀性高、也方便後續除錯或驗證。相比非遞迴版本需要額外的堆疊模擬，遞迴的結構更自然，能讓程式邏輯一目了然。

2. 符合數學定義與邏輯直觀性  
   Ackermann 函數本身就是以遞迴方式定義的，因此使用遞迴函式來實作最能直接反映其數學結構。每一層呼叫都清楚對應到定義式中的三種情況：  
   當 $m = 0$ 時，回傳 $n+1$ 。  
   當 $n =0$ 時，計算 $A(m-1,1)$ 。  
   其他情況下，呼叫 $A(m-1,A(m,n-1)) 。  
   這樣的邏輯層次清晰、概念明確，讓整體函式架構與原始定義完全一致。
