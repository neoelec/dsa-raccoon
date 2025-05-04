// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
//

// Doubly Linked List
#ifndef __RCN_CPP_DLIST_H__
#define __RCN_CPP_DLIST_H__

#include <cstddef>

namespace rcn_cpp
{

template <typename T>
struct dlnode {
    dlnode<T> *prev_;
    dlnode<T> *next_;
    T *entry_;

    void __insert_after(dlnode<T> *x)
    {
        x->prev_ = this;
        x->next_ = next_;
        next_->prev_ = x;
        next_ = x;
    }

    void insert_after(dlnode<T> *x, T *e)
    {
        x->entry_ = e;
        __insert_after(x);
    }

    T *erase()
    {
        prev_->next_ = next_;
        next_->prev_ = prev_;
        return entry_;
    }
};

template <typename T>
class dlist {
public:
    dlist()
    {
        clear();
    }

    void clear()
    {
        pool_.prev_ = pool_.next_ = &pool_;
    }

    void init()
    {
        clear();
    }

    dlnode<T> *begin() const
    {
        return pool_.next_;
    }

    const dlnode<T> *end() const
    {
        return &pool_;
    }

    bool empty() const
    {
        return begin() == &pool_;
    }

    size_t size() const
    {
        dlnode<T> *x;
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

    T *back() const
    {
        return empty() ? nullptr : end()->prev_->entry_;
    }

    void push_front(dlnode<T> *x, T *e)
    {
        pool_.insert_after(x, e);
    }

    void push_back(dlnode<T> *x, T *e)
    {
        end()->prev_->insert_after(x, e);
    }

    T *pop_front()
    {
        return begin()->erase();
    }

    T *pop_back()
    {
        return end()->prev_->erase();
    }

    T *at(size_t n) const
    {
        dlnode<T> *x;
        for (x = begin(); n != 0 && x != end(); --n) {
            x = x->next_;
        }
        return x == end() ? nullptr : x->entry_;
    }

    void swap(dlist<T> *other)
    {
        dlnode<T> head;
        pool_.__insert_after(&head);
        pool_.erase();
        other->pool_.__insert_after(&pool_);
        other->pool_.erase();
        head.__insert_after(&other->pool_);
        head.erase();
    }

    dlnode<T> *remove_if(const T *ke, int (*compar)(const T *ke, const T *e))
    {
        dlnode<T> *x;
        for (x = begin(); x != end(); x = x->next_) {
            if (compar(ke, x->entry_) == 0) {
                x->erase();
                return x;
            }
        }
        return nullptr;
    }

    void reverse()
    {
        dlnode<T> tmp = { .prev_ = &tmp, .next_ = &tmp, .entry_ = nullptr };
        while (!empty()) {
            dlnode<T> *x = begin();
            pop_front();
            tmp.__insert_after(x);
        }
        tmp.__insert_after(&pool_);
        tmp.erase();
    }

private:
    dlnode<T> pool_;
};

}

#endif // __RCN_CPP_DLIST_H__
