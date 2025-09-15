/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Hash Table - Separate Chaining (Singly Linked List) */
#ifndef __RCN_C_SLHASH_H__
#define __RCN_C_SLHASH_H__

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "slist.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct slhash {
    size_t sz_bucket_;
    size_t (*key_hash_)(const void *ke);
    int (*compar_)(const void *ke, const void *in_table);
    struct slist *bucket_;
};

static inline size_t __slhash_index(const struct slhash *table, const void *ke)
{
    return table->key_hash_(ke) % table->sz_bucket_;
}

static inline void slhash_clear(struct slhash *self)
{
    if (self->bucket_ == NULL) {
        return;
    }

    for (size_t i = 0; i < self->sz_bucket_; ++i) {
        slist_clear(&self->bucket_[i]);
    }
}

static inline void slhash_init(
    struct slhash *self, size_t sz_bucket, size_t (*key_hash)(const void *ke),
    int (*compar)(const void *ke, const void *in_table), struct slist *bucket)
{
    self->sz_bucket_ = sz_bucket;
    self->key_hash_ = key_hash;
    self->compar_ = compar;
    self->bucket_ = bucket;
    slhash_clear(self);
}

static inline size_t slhash_buckets(const struct slhash *self)
{
    return self->sz_bucket_;
}

static inline struct slnode *slhash_begin(const struct slhash *self, size_t n)
{
    return slist_begin(&self->bucket_[n]);
}

static inline const struct slnode *slhash_end(const struct slhash *self,
                                              size_t n)
{
    return slist_end(&self->bucket_[n]);
}

static inline bool slhash_empty(const struct slhash *self)
{
    for (size_t i = 0; i < slhash_buckets(self); ++i) {
        if (!slist_empty(&self->bucket_[i])) {
            return false;
        }
    }

    return true;
}

static inline size_t slhash_size(const struct slhash *self)
{
    size_t count = 0;

    for (size_t i = 0; i < slhash_buckets(self); ++i) {
        count += slist_size(&self->bucket_[i]);
    }

    return count;
}

static inline int slhash_insert(struct slhash *self, struct slnode *z, void *e)
{
    size_t n = __slhash_index(self, e);
    struct slist *bucket = &self->bucket_[n];

    for (struct slnode *x = slist_begin(bucket); x != slist_end(bucket);
         x = x->next_) {
        if (self->compar_(e, x->entry_) == 0) {
            return -EEXIST;
        }
    }

    slist_push_front(bucket, z, e);

    return 0;
}

static inline void slhash_insert_multi(struct slhash *self, struct slnode *z,
                                       void *e)
{
    size_t n = __slhash_index(self, e);
    struct slist *bucket = &self->bucket_[n];

    slist_push_front(bucket, z, e);
}

static inline struct slnode *slhash_find(const struct slhash *self,
                                         const void *ke)
{
    struct slnode *x;
    size_t n = __slhash_index(self, ke);

    for (x = slhash_begin(self, n); x != slhash_end(self, n); x = x->next_) {
        if (self->compar_(ke, x->entry_) == 0) {
            return x;
        }
    }

    return NULL;
}

static inline void *slhash_at(const struct slhash *self, const void *ke)
{
    struct slnode *x = slhash_find(self, ke);

    return x == NULL ? NULL : x->entry_;
}

static inline size_t slhash_size(const struct slhash *self, const void *ke)
{
    size_t n = __slhash_index(self, ke);

    return slist_size(&self->bucket_[n]);
}

static inline void slhash_swap(struct slhash *self, struct slhash *other)
{
    struct slhash tmp = *self;

    *self = *other;
    *other = tmp;
}

static inline int __slhash_alloc_bucket(struct slhash *self)
{
    self->bucket_ =
        (struct slist *)calloc(self->sz_bucket_, sizeof(*self->bucket_));
    slhash_clear(self);
    return self->bucket_ == NULL ? -ENOMEM : 0;
}

static inline void __slhash_free_bucket(struct slhash *self)
{
    free(self->bucket_);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_SLHASH_H__ */
