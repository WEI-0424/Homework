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
