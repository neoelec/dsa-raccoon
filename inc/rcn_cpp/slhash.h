// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
//

// Hash Table - Separate Chaining (Doubly Linked List)
#ifndef __RCN_CPP_DLHASH_H__
#define __RCN_CPP_DLHASH_H__

#include <cerrno>
#include <cstring>

#include "slist.h"

namespace rcn_cpp
{

template <typename T>
class slhash {
public:
    slhash()
    {
    }

    slhash(size_t sz_bucket, size_t (*key_hash)(const T *ke),
           int (*compar)(const T *ke, const T *in_table),
           slist<T> *bucket = nullptr)
        : sz_bucket_(sz_bucket)
        , key_hash_(key_hash)
        , compar_(compar)
        , bucket_(bucket)
    {
        clear();
    }

    void clear()
    {
        if (bucket_ == nullptr) {
            return;
        }
        for (size_t i = 0; i < sz_bucket_; ++i) {
            bucket_[i].clear();
        }
    }

    size_t buckets() const
    {
        return sz_bucket_;
    }

    slnode<T> *begin(size_t n) const
    {
        return bucket_[n].begin();
    }

    const slnode<T> *end(size_t n) const
    {
        return bucket_[n].end();
    }

    bool empty() const
    {
        for (size_t i = 0; i < buckets(); ++i) {
            if (!bucket_[i].empty()) {
                return false;
            }
        }
        return true;
    }

    size_t size() const
    {
        size_t count = 0;
        for (size_t i = 0; i < buckets(); ++i) {
            count += bucket_[i].size();
        }
        return count;
    }

    void insert(slnode<T> *x, T *e)
    {
        size_t n = __index(e);
        bucket_[n].push_front(x, e);
    }

    slnode<T> *find(const T *ke) const
    {
        slnode<T> *x;
        size_t n = __index(ke);
        for (x = begin(n); x != end(n); x = x->next_) {
            if (compar_(ke, x->entry_) == 0) {
                return x;
            }
        }
        return nullptr;
    }

    T *at(const T *ke) const
    {
        slnode<T> *x = find(ke);
        return x == nullptr ? nullptr : x->entry_;
    }

    size_t count(const T *ke) const
    {
        size_t n = __index(ke);
        return bucket_[n].size();
    }

    void swap(slhash<T> *other)
    {
        slhash<T> tmp;
        memcpy(&tmp, this, sizeof(tmp));
        memcpy(this, other, sizeof(tmp));
        memcpy(other, &tmp, sizeof(tmp));
    }

    int alloc_bucket()
    {
        bucket_ = new slist<T>[sz_bucket_];
        clear();
        return bucket_ == nullptr ? -ENOMEM : 0;
    }

    void free_bucket()
    {
        delete bucket_;
    }

private:
    inline size_t __index(const T *ke) const
    {
        return key_hash_(ke) % sz_bucket_;
    }

private:
    size_t sz_bucket_;
    size_t (*key_hash_)(const T *ke);
    int (*compar_)(const T *ke, const T *in_table);
    slist<T> *bucket_;
};

}

#endif // __RCN_CPP_DLHASH_H__
