// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
//

// Singly Linked List
#ifndef __RCN_CPP_SLIST_H__
#define __RCN_CPP_SLIST_H__

#include <cstddef>

namespace rcn_cpp
{

template <typename T>
struct slnode {
    slnode<T> *next_;
    T *entry_;

    void __insert_after(slnode<T> *x)
    {
        x->next_ = next_;
        next_ = x;
    }

    void insert_after(slnode<T> *x, T *e)
    {
        x->entry_ = e;
        __insert_after(x);
    }

    T *erase_after()
    {
        if (next_ == nullptr) {
            return nullptr;
        }
        slnode<T> *x = next_;
        next_ = x->next_;
        return x->entry_;
    }
};

template <typename T>
class slist {
public:
    slist()
    {
        clear();
    }

    void clear()
    {
        pool_.next_ = nullptr;
    }

    slnode<T> *begin() const
    {
        return pool_.next_;
    }

    slnode<T> *end() const
    {
        return nullptr;
    }

    bool empty() const
    {
        return begin() == nullptr;
    }

    size_t size() const
    {
        slnode<T> *x;
        size_t count = 0;
        for (x = begin(); x != end(); x = x->next_) {
            count++;
        }
        return count;
    }

    T *front() const
    {
        return empty() ? nullptr : begin()->entry_;
    }

    void push_front(slnode<T> *x, T *e)
    {
        pool_.insert_after(x, e);
    }

    T *pop_front()
    {
        return pool_.erase_after();
    }

    T *at(size_t n) const
    {
        slnode<T> *x;
        for (x = begin(); n != 0 && x != end(); --n) {
            x = x->next_;
        }
        return x == end() ? nullptr : x->entry_;
    }

    void swap(slist<T> *other)
    {
        slnode<T> tmp;
        tmp.next_ = begin();
        pool_.next_ = other->begin();
        other->pool_.next_ = tmp.next_;
    }

    slnode<T> *remove_if(const T *ke, int (*compar)(const T *ke, const T *e))
    {
        slnode<T> *p, *x;
        p = &pool_;
        for (x = begin(); x != end(); p = x, x = x->next_) {
            if (compar(ke, x->entry_) == 0) {
                p->erase_after();
                return x;
            }
        }
        return nullptr;
    }

    void reverse()
    {
        slnode<T> tmp = { .next_ = nullptr, .entry_ = nullptr };
        while (!empty()) {
            slnode<T> *x = begin();
            pop_front();
            tmp.__insert_after(x);
        }
        pool_.next_ = tmp.next_;
    }

private:
    slnode<T> pool_;
};

}

#endif // __RCN_CPP_SLIST_H__
