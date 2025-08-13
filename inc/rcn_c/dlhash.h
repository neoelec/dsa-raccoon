/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Hash Table - Separate Chaining (Doubly Linked List) */
#ifndef __RCN_C_DLHASH_H__
#define __RCN_C_DLHASH_H__

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct dlhash {
    size_t sz_bucket_;
    size_t (*key_hash_)(const void *ke);
    int (*compar_)(const void *ke, const void *in_table);
    struct dlist *bucket_;
};

static inline size_t __dlhash_index(const struct dlhash *self, const void *ke)
{
    return self->key_hash_(ke) % self->sz_bucket_;
}

static inline void dlhash_clear(struct dlhash *self)
{
    if (self->bucket_ == NULL) {
        return;
    }

    for (size_t i = 0; i < self->sz_bucket_; ++i) {
        dlist_clear(&self->bucket_[i]);
    }
}

static inline void dlhash_init(
    struct dlhash *self, size_t sz_bucket, size_t (*key_hash)(const void *ke),
    int (*compar)(const void *ke, const void *in_table), struct dlist *bucket)
{
    self->sz_bucket_ = sz_bucket;
    self->key_hash_ = key_hash;
    self->compar_ = compar;
    self->bucket_ = bucket;
    dlhash_clear(self);
}

static inline size_t dlhash_buckets(const struct dlhash *self)
{
    return self->sz_bucket_;
}

static inline struct dlnode *dlhash_begin(const struct dlhash *self, size_t n)
{
    return dlist_begin(&self->bucket_[n]);
}

static inline const struct dlnode *dlhash_end(const struct dlhash *self,
                                              size_t n)
{
    return dlist_end(&self->bucket_[n]);
}

static inline bool dlhash_empty(const struct dlhash *self)
{
    for (size_t i = 0; i < dlhash_buckets(self); ++i) {
        if (!dlist_empty(&self->bucket_[i])) {
            return false;
        }
    }

    return true;
}

static inline size_t dlhash_size(const struct dlhash *self)
{
    size_t count = 0;

    for (size_t i = 0; i < dlhash_buckets(self); ++i) {
        count += dlist_size(&self->bucket_[i]);
    }

    return count;
}

static inline void dlhash_insert(struct dlhash *self, struct dlnode *x, void *e)
{
    size_t n = __dlhash_index(self, e);

    dlist_push_front(&self->bucket_[n], x, e);
}

static inline void *dlhash_erase(struct dlhash *self, struct dlnode *x)
{
    return dlist_erase(x);
}

static inline struct dlnode *dlhash_find(const struct dlhash *self,
                                         const void *ke)
{
    struct dlnode *x;
    size_t n = __dlhash_index(self, ke);

    for (x = dlhash_begin(self, n); x != dlhash_end(self, n); x = x->next_) {
        if (self->compar_(ke, x->entry_) == 0) {
            return x;
        }
    }

    return NULL;
}

static inline void *dlhash_at(const struct dlhash *self, const void *ke)
{
    struct dlnode *x = dlhash_find(self, ke);

    return x == NULL ? NULL : x->entry_;
}

static inline struct dlnode *dlhash_remove(struct dlhash *self, const void *ke)
{
    struct dlnode *x = dlhash_find(self, ke);

    if (x == NULL) {
        return NULL;
    }

    dlhash_erase(self, x);
    return x;
}

static inline size_t dlhash_count(const struct dlhash *self, const void *ke)
{
    size_t n = __dlhash_index(self, ke);

    return dlist_size(&self->bucket_[n]);
}

static inline void dlhash_swap(struct dlhash *self, struct dlhash *other)
{
    struct dlhash tmp;

    memcpy(&tmp, self, sizeof(tmp));
    memcpy(self, other, sizeof(tmp));
    memcpy(other, &tmp, sizeof(tmp));
}

static inline int __dlhash_alloc_bucket(struct dlhash *self)
{
    self->bucket_ =
        (struct dlist *)calloc(self->sz_bucket_, sizeof(*self->bucket_));
    dlhash_clear(self);
    return self->bucket_ == NULL ? -ENOMEM : 0;
}

static inline void __dlhash_free_bucket(struct dlhash *self)
{
    free(self->bucket_);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_DLHASH_H__ */
