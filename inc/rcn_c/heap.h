/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Heap (Array) */
#ifndef __RCN_C_HEAP_H__
#define __RCN_C_HEAP_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct heap {
    size_t nr_entries_;
    int (*compar_)(const void *ke, const void *in_heap);
    void **entry_;
    volatile size_t size_;
};

static inline size_t __heap_parent(size_t pos)
{
    return (pos - 1) / 2;
}

static inline size_t __heap_left(size_t pos)
{
    return (pos * 2) + 1;
}

static inline int __heap_compar(const struct heap *self, size_t ke_i,
                                size_t in_heap_i)
{
    return self->compar_(self->entry_[ke_i], self->entry_[in_heap_i]);
}

static inline void __heap_swap(struct heap *self, size_t ke_i, size_t in_heap_i)
{
    void *tmp = self->entry_[ke_i];

    self->entry_[ke_i] = self->entry_[in_heap_i];
    self->entry_[in_heap_i] = tmp;
}

static inline void heap_clear(struct heap *self)
{
    self->size_ = 0;
}

static inline void heap_init(struct heap *self, size_t nr_entries,
                             int (*compar)(const void *ke, const void *in_heap),
                             void **entry)
{
    self->nr_entries_ = nr_entries;
    self->compar_ = compar;
    self->entry_ = entry;
    heap_clear(self);
}

static inline size_t heap_size(const struct heap *self)
{
    return self->size_;
}

static inline size_t heap_max_size(const struct heap *self)
{
    return self->nr_entries_;
}

static inline bool heap_empty(const struct heap *self)
{
    return heap_size(self) == 0;
}

static inline bool heap_full(const struct heap *self)
{
    return heap_size(self) == self->nr_entries_;
}

static inline void *heap_top(const struct heap *self)
{
    return heap_empty(self) ? NULL : self->entry_[0];
}

static inline int heap_push(struct heap *self, void *e)
{
    if (heap_full(self)) {
        return -ENOSPC;
    }

    ssize_t pos, parent;
    pos = self->size_++;
    parent = __heap_parent(pos);

    while (pos > 0 && self->compar_(e, self->entry_[parent]) < 0) {
        self->entry_[pos] = self->entry_[parent];
        pos = parent;
        parent = __heap_parent(pos);
    }

    self->entry_[pos] = e;

    return 0;
}

static inline void *heap_pop(struct heap *self)
{
    if (heap_empty(self)) {
        return NULL;
    }

    size_t pos, child, left, right;
    void *e = self->entry_[0];
    self->entry_[0] = self->entry_[--self->size_];
    pos = 0;
    left = __heap_left(pos);

    while (left < self->size_) {
        right = left + 1;

        if (right == self->size_) {
            child = left;
        } else {
            child = __heap_compar(self, left, right) < 0 ? left : right;
        }

        if (__heap_compar(self, pos, child) < 0) {
            break;
        }

        __heap_swap(self, pos, child);
        pos = child;
        left = __heap_left(pos);
    }

    return e;
}

static inline void heap_swap(struct heap *self, struct heap *other)
{
    struct heap tmp;

    memcpy(&tmp, self, sizeof(tmp));
    memcpy(self, other, sizeof(tmp));
    memcpy(other, &tmp, sizeof(tmp));
}

static inline int __heap_alloc_entry(struct heap *self)
{
    self->entry_ = (void **)calloc(self->nr_entries_, sizeof(*self->entry_));
    return self->entry_ == NULL ? -ENOMEM : 0;
}

static inline void __heap_free_entry(struct heap *self)
{
    free(self->entry_);
}

static inline bool heap_validate(const struct heap *self)
{
    for (size_t i = 2; i < self->size_; i++) {
        size_t left = __heap_left(i);
        size_t right = left + 1;

        if ((left < self->size_) && (__heap_compar(self, i, left) < 0)) {
            return false;
        } else if ((right < self->size_) &&
                   (__heap_compar(self, i, right) > 0)) {
            return false;
        }
    }

    return true;
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_HEAP_H__ */
