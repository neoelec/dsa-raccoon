/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Stack (Array) */
#ifndef __RCN_CPP_STACK_H__
#define __RCN_CPP_STACK_H__

#include <cerrno>
#include <cstddef>
#include <cstring>

namespace rcn_cpp
{
template <typename T>
class stack {
public:
    stack()
    {
    }

    stack(size_t nr_entries, T **entry = nullptr)
        : nr_entries_(nr_entries)
        , entry_(entry)
    {
        clear();
    }

    void clear()
    {
        top_ = -1;
    }

    size_t size() const
    {
        return top_ + 1;
    }

    size_t max_size() const
    {
        return nr_entries_;
    }

    bool empty() const
    {
        return size() == 0;
    }

    bool full() const
    {
        return size() == nr_entries_;
    }

    T *top() const
    {
        if (empty()) {
            return nullptr;
        }

        return entry_[top_];
    }

    void push(T *e)
    {
        if (size() == nr_entries_) {
            return;
        }

        entry_[++top_] = e;
    }

    T *pop()
    {
        if (empty()) {
            return nullptr;
        }

        return entry_[top_--];
    }

    void swap(stack *other)
    {
        stack tmp;

        memcpy(&tmp, this, sizeof(tmp));
        memcpy(this, other, sizeof(tmp));
        memcpy(other, &tmp, sizeof(tmp));
    }

    int alloc_entry()
    {
        entry_ = new T *[nr_entries_];
        return entry_ == nullptr ? -ENOMEM : 0;
    }

    void free_entry()
    {
        delete entry_;
    }

private:
    size_t nr_entries_;
    T **entry_;
    size_t top_;
};
}

#endif /* __RCN_CPP_STACK_H__ */
