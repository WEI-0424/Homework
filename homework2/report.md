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
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream &output, const Polynomial &Poly);
private:
    int exp;
    float coef;
};

class Polynomial {
private:
    Term *termArray;
    int capacity;
    int terms;
public:
    Polynomial(): capacity(2), terms(0) {
        termArray = new Term[capacity];
    }
    ~Polynomial() {
        delete [] termArray;
    }

    Polynomial(const Polynomial& other): capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }

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

    Polynomial Add(const Polynomial& b) const;
    void newTerm(const float newcoef, const int newexp);

    friend istream& operator>>(istream& is, Polynomial& poly);
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
};

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

ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; ++i) {
        if (i > 0) os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}

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
    cout << c << endl;
    return 0;
}

}
```


## 效能分析
遞迴函式:
1. 時間複雜度：當 $m$ 與 $n$　的數值較小時(例如 $m=2,n=4$ )，整體遞迴深度仍然可控制在安全範圍內;但隨著輸入增大，遞迴呼叫會迅速堆疊，導致記憶體占用成倍成長。當 $m=0,1,2$ 時，空間複雜度大約為 $O(1)$ 、 $O(2)$ 、 $O(3)$ ;若m=3，則上升至約 $O(2ⁿ)$ ；而當 $m≥4$ 時，成長速度甚至超越指數級，極容易造成系統堆疊溢位。
2. 空間複雜度：對於較小的 $m$ 值(例如 $0、1、2、3$ )，可藉由簡單的遞迴公式推得其時間複雜度分別約為 $O(1)$ 、 $O(n)$ 、 $O(2ⁿ)$ 。但當 $m$ 提升至 4 以上時，函式的執行次數呈現爆炸性增長，其增長速率遠超過一般指數級，理論上屬於超指數等級的複雜度。

非遞迴函式:
1. 時間複雜度：在非遞迴版本中，雖然以陣列模擬呼叫堆疊能避免系統層級的遞迴限制，但所需的空間量仍隨m與n的增長急劇膨脹。當 $m=3$ 時，約為 $O(n)$ ;當m=4時，則提升為 $O(2ⁿ)$ ;而若 $m≥5$ ，其空間需求甚至可達 $O(2^(2^(⋯2^n)))$ 的多層次指數規模。
2. 空間複雜度：非遞迴實作的時間成本與遞迴版本相當，因為運算邏輯一致。當 $m=3$ 時，時間複雜度約為 $O(2ⁿ)$；當 $m=4$ 時，則成長為雙層指數等級 $O(2^(2^(⋯2^n)))$ 。實際測試中，例如 $A(4,2)$ 的計算就可能需要數億次以上的操作，顯示其運算量極為龐大。
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $m = 0$, $n = 3$ | $4$    | $4$        |
| 測試二   | $m = 1$, $n = 4$ | $6$        | $6$        |
| 測試三   | $m = 2$, $n = 3$ | $9$        | $9$        |
| 測試四   | $m = 3$, $n = 3$ | $61$       | $61$       |
| 測試五   | $m = 4$, $n = 0$ | 異常輸出 | 異常輸出 |

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
