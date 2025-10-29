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
