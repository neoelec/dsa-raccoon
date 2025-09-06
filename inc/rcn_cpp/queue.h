/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Queue (Array) */
#ifndef __RCN_CPP_QUEUE_H__
#define __RCN_CPP_QUEUE_H__

#include <cerrno>
#include <cstddef>
#include <cstring>

namespace rcn_cpp
{
template <typename T>
class queue {
public:
    queue()
    {
    }

    queue(size_t nr_entries, T **entry = nullptr)
        : nr_entries_(nr_entries)
        , entry_(entry)
    {
        clear();
    }

    void clear()
    {
        front_ = nr_entries_ - 1;
        back_ = 0;
        size_ = 0;
    }

    size_t size() const
    {
        return size_;
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

    T *front() const
    {
        return empty() ? nullptr : entry_[front_];
    }

    T *back() const
    {
        return empty() ? nullptr : entry_[back_];
    }

    void push(T *e)
    {
        if (full()) {
            return;
        }

        back_ = back_ == 0 ? nr_entries_ - 1 : back_ - 1;
        entry_[back_] = e;
        size_++;
    }

    T *pop()
    {
        if (empty()) {
            return nullptr;
        }

        T *e = entry_[front_];
        front_ = front_ == 0 ? nr_entries_ - 1 : front_ - 1;
        size_--;
        return e;
    }

    void swap(queue *other)
    {
        queue tmp;

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
    size_t front_;
    size_t back_;
    volatile size_t size_;
};
}

#endif /* __RCN_CPP_QUEUE_H__ */
