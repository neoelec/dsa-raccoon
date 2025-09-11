/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Heap (Array) */
#ifndef __RCN_CPP_HEAP_H__
#define __RCN_CPP_HEAP_H__

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <sys/types.h>

namespace rcn_cpp
{
template <typename T>
class heap {
public:
    heap()
    {
    }

    heap(size_t nr_entries, int (*compar)(const T *ke, const T *in_heap),
         T **entry = nullptr)
        : nr_entries_(nr_entries)
        , compar_(compar)
        , entry_(entry)
    {
        clear();
    }

    void clear()
    {
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

    T *top() const
    {
        return empty() ? nullptr : entry_[0];
    }

    void push(T *e)
    {
        if (full()) {
            return;
        }

        ssize_t pos, parent;
        pos = size_++;
        parent = __parent(pos);

        while (pos > 0 && compar_(e, entry_[parent]) < 0) {
            entry_[pos] = entry_[parent];
            pos = parent;
            parent = __parent(pos);
        }

        entry_[pos] = e;
    }

    T *pop()
    {
        if (empty()) {
            return nullptr;
        }

        size_t pos, child, left, right;
        T *e = entry_[0];
        entry_[0] = entry_[--size_];
        pos = 0;
        left = __left(pos);

        while (left < size_) {
            right = left + 1;

            if (right == size_) {
                child = left;
            } else {
                child = __compar(left, right) < 0 ? left : right;
            }

            if (__compar(pos, child) < 0) {
                break;
            }

            __swap(pos, child);
            pos = child;
            left = __left(pos);
        }

        return e;
    }

    void swap(heap *other)
    {
        heap tmp;

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
    inline size_t __parent(size_t pos)
    {
        return (pos - 1) / 2;
    }

    inline size_t __left(size_t pos)
    {
        return (pos * 2) + 1;
    }

    inline int __compar(size_t ke_i, size_t in_heap_i) const
    {
        return compar_(entry_[ke_i], entry_[in_heap_i]);
    }

    inline void __swap(size_t ke_i, size_t in_heap_i)
    {
        T *tmp = entry_[ke_i];

        entry_[ke_i] = entry_[in_heap_i];
        entry_[in_heap_i] = tmp;
    }

private:
    size_t nr_entries_;
    int (*compar_)(const T *e, const T *in_heap);
    T **entry_;
    volatile size_t size_;
};
}

#endif /* __RCN_CPP_HEAP_H__ */
