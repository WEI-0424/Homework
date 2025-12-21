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

