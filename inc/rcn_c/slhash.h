/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Hash Table - Separate Chaining (Singly Linked List) */
#ifndef __RCN_C_SLHASH_H__
#define __RCN_C_SLHASH_H__

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "slist.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct slhash {
    size_t nr_buckets_;
    size_t (*key_hash_)(const void *ke);
    int (*compar_)(const void *ke, const void *in_table);
    struct slist *bucket_;
    size_t size_;
};

static inline size_t __slhash_index(const struct slhash *table, const void *ke)
{
    return table->key_hash_(ke) % table->nr_buckets_;
}

static inline void slhash_clear(struct slhash *self)
{
    self->size_ = 0;

    if (self->bucket_ == NULL) {
        return;
    }

    for (size_t i = 0; i < self->nr_buckets_; ++i) {
        slist_clear(&self->bucket_[i]);
    }
}

static inline void slhash_init(
    struct slhash *self, size_t sz_bucket, size_t (*key_hash)(const void *ke),
    int (*compar)(const void *ke, const void *in_table), struct slist *bucket)
{
    self->nr_buckets_ = sz_bucket;
    self->key_hash_ = key_hash;
    self->compar_ = compar;
    self->bucket_ = bucket;
    slhash_clear(self);
}

static inline size_t slhash_buckets(const struct slhash *self)
{
    return self->nr_buckets_;
}

static inline const struct slnode *slhash_end(const struct slhash *self)
{
    return slist_end(&self->bucket_[self->nr_buckets_ - 1]);
}

static inline struct slnode *slhash_begin(const struct slhash *self)
{
    for (size_t n = 0; n < self->nr_buckets_; n++) {
        struct slist *list = &self->bucket_[n];

        if (!slist_empty(list)) {
            return slist_begin(list);
        }
    }

    return (struct slnode *)slhash_end(self);
}

static inline struct slnode *slhash_next(const struct slhash *self,
                                         const struct slnode *x)
{
    if (x->next_ != slist_end(x->list_)) {
        return x->next_;
    }

    for (size_t m = 1; (uintptr_t)(&x->list_[m]) < (uintptr_t)slhash_end(self);
         m++) {
        if (!slist_empty(&x->list_[m])) {
            return slist_begin(&x->list_[m]);
        }
    }

    return (struct slnode *)slhash_end(self);
}

static inline size_t slhash_size(const struct slhash *self)
{
    return self->size_;
}

static inline bool slhash_empty(const struct slhash *self)
{
    return slhash_size(self) == 0;
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
    self->size_++;

    return 0;
}

static inline void slhash_insert_multi(struct slhash *self, struct slnode *z,
                                       void *e)
{
    size_t n = __slhash_index(self, e);
    struct slist *bucket = &self->bucket_[n];

    slist_push_front(bucket, z, e);
    self->size_++;
}

static inline void *slhash_erase(struct slhash *self, struct slnode *z)
{
    self->size_--;
    return slist_erase(z);
}

static inline struct slnode *slhash_find(const struct slhash *self,
                                         const void *ke)
{
    struct slnode *x;
    size_t n = __slhash_index(self, ke);
    struct slist *list = &self->bucket_[n];

    for (x = slist_begin(list); x != slist_end(list); x = x->next_) {
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

static inline bool slhash_validate(struct slhash *self)
{
    size_t size = 0;

    for (size_t n = 0; n < self->nr_buckets_; n++) {
        if (!slist_validate(&self->bucket_[n])) {
            return false;
        }

        size += slist_size(&self->bucket_[n]);
    }

    if (slhash_size(self) != size) {
        return false;
    }

    size = 0;

    for (const struct slnode *x = slhash_begin(self); x != slhash_end(self);
         x = slhash_next(self, x)) {
        size++;
    }

    return slhash_size(self) == size;
}

static inline int __slhash_alloc_bucket(struct slhash *self)
{
    self->bucket_ =
        (struct slist *)calloc(self->nr_buckets_, sizeof(*self->bucket_));
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
