/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Hash Table - Separate Chaining (Doubly Linked List) */
#ifndef __RCN_C_DLHASH_H__
#define __RCN_C_DLHASH_H__

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct dlhash {
    size_t nr_buckets_;
    size_t (*key_hash_)(const void *ke);
    int (*compar_)(const void *ke, const void *in_table);
    struct dlist *bucket_;
    size_t size_;
};

static inline size_t __dlhash_index(const struct dlhash *self, const void *ke)
{
    return self->key_hash_(ke) % self->nr_buckets_;
}

static inline void dlhash_clear(struct dlhash *self)
{
    self->size_ = 0;

    if (self->bucket_ == NULL) {
        return;
    }

    for (size_t i = 0; i < self->nr_buckets_; ++i) {
        dlist_clear(&self->bucket_[i]);
    }
}

static inline void dlhash_init(
    struct dlhash *self, size_t sz_bucket, size_t (*key_hash)(const void *ke),
    int (*compar)(const void *ke, const void *in_table), struct dlist *bucket)
{
    self->nr_buckets_ = sz_bucket;
    self->key_hash_ = key_hash;
    self->compar_ = compar;
    self->bucket_ = bucket;
    dlhash_clear(self);
}

static inline size_t dlhash_buckets(const struct dlhash *self)
{
    return self->nr_buckets_;
}

static inline const struct dlnode *dlhash_end(const struct dlhash *self)
{
    return dlist_end(&self->bucket_[self->nr_buckets_ - 1]);
}

static inline struct dlnode *dlhash_begin(const struct dlhash *self)
{
    for (size_t n = 0; n < self->nr_buckets_; n++) {
        struct dlist *list = &self->bucket_[n];

        if (!dlist_empty(list)) {
            return dlist_begin(list);
        }
    }

    return (struct dlnode *)dlhash_end(self);
}

static inline struct dlnode *dlhash_next(const struct dlhash *self,
                                         const struct dlnode *x)
{
    if (x->next_ != dlist_end(x->list_)) {
        return x->next_;
    }

    for (size_t m = 1; (uintptr_t)(&x->list_[m]) < (uintptr_t)dlhash_end(self);
         m++) {
        if (!dlist_empty(&x->list_[m])) {
            return dlist_begin(&x->list_[m]);
        }
    }

    return (struct dlnode *)dlhash_end(self);
}

static inline size_t dlhash_size(const struct dlhash *self)
{
    return self->size_;
}

static inline bool dlhash_empty(const struct dlhash *self)
{
    return dlhash_size(self) == 0;
}

static inline int dlhash_insert(struct dlhash *self, struct dlnode *z, void *e)
{
    size_t n = __dlhash_index(self, e);
    struct dlist *bucket = &self->bucket_[n];

    for (struct dlnode *x = dlist_begin(bucket); x != dlist_end(bucket);
         x = x->next_) {
        if (self->compar_(e, x->entry_) == 0) {
            return -EEXIST;
        }
    }

    dlist_push_front(bucket, z, e);
    self->size_++;

    return 0;
}

static inline void dlhash_insert_multi(struct dlhash *self, struct dlnode *z,
                                       void *e)
{
    size_t n = __dlhash_index(self, e);
    struct dlist *bucket = &self->bucket_[n];

    dlist_push_front(bucket, z, e);
    self->size_++;
}

static inline void *dlhash_erase(struct dlhash *self, struct dlnode *z)
{
    self->size_--;
    return dlist_erase(z);
}

static inline struct dlnode *dlhash_find(const struct dlhash *self,
                                         const void *ke)
{
    struct dlnode *x;
    size_t n = __dlhash_index(self, ke);
    struct dlist *list = &self->bucket_[n];

    for (x = dlist_begin(list); x != dlist_end(list); x = x->next_) {
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

static inline void dlhash_swap(struct dlhash *self, struct dlhash *other)
{
    struct dlhash tmp = *self;

    *self = *other;
    *other = tmp;
}

static inline bool dlhash_validate(struct dlhash *self)
{
    size_t size = 0;

    for (size_t n = 0; n < self->nr_buckets_; n++) {
        if (!dlist_validate(&self->bucket_[n])) {
            return false;
        }

        size += dlist_size(&self->bucket_[n]);
    }

    if (dlhash_size(self) != size) {
        return false;
    }

    size = 0;

    for (const struct dlnode *x = dlhash_begin(self); x != dlhash_end(self);
         x = dlhash_next(self, x)) {
        size++;
    }

    return dlhash_size(self) == size;
}

static inline int __dlhash_alloc_bucket(struct dlhash *self)
{
    self->bucket_ =
        (struct dlist *)calloc(self->nr_buckets_, sizeof(*self->bucket_));
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
