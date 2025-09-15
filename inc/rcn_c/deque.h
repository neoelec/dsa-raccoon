/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Deque (Array) */
#ifndef __RCN_C_DEQUE_H__
#define __RCN_C_DEQUE_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct deque {
    size_t nr_entries_;
    void **entry_;
    size_t front_;
    size_t back_;
    volatile size_t size_;
};

static inline void deque_clear(struct deque *self)
{
    self->front_ = self->nr_entries_ - 1;
    self->back_ = 0;
    self->size_ = 0;
}

static inline void deque_init(struct deque *self, size_t nr_entries,
                              void **entry)
{
    self->nr_entries_ = nr_entries;
    self->entry_ = entry;
    deque_clear(self);
}

static inline size_t deque_size(const struct deque *self)
{
    return self->size_;
}

static inline size_t deque_max_size(const struct deque *self)
{
    return self->nr_entries_;
}

static inline bool deque_empty(const struct deque *self)
{
    return deque_size(self) == 0;
}

static inline bool deque_full(const struct deque *self)
{
    return deque_size(self) == self->nr_entries_;
}

static inline void *deque_front(const struct deque *self)
{
    return deque_empty(self) ? NULL : self->entry_[self->front_];
}

static inline void *deque_back(const struct deque *self)
{
    return deque_empty(self) ? NULL : self->entry_[self->back_];
}

static inline int deque_push_front(struct deque *self, void *e)
{
    if (deque_full(self)) {
        return -ENOSPC;
    }

    self->front_++;

    if (self->front_ == self->nr_entries_) {
        self->front_ = 0;
    }

    self->entry_[self->front_] = e;
    self->size_++;

    return 0;
}

static inline int deque_push_back(struct deque *self, void *e)
{
    if (deque_full(self)) {
        return -ENOSPC;
    }

    self->back_ = self->back_ == 0 ? self->nr_entries_ - 1 : self->back_ - 1;
    self->entry_[self->back_] = e;
    self->size_++;

    return 0;
}

static inline void *deque_pop_front(struct deque *self)
{
    if (deque_empty(self)) {
        return NULL;
    }

    void *e = self->entry_[self->front_];
    self->front_ = self->front_ == 0 ? self->nr_entries_ - 1 : self->front_ - 1;
    self->size_--;
    return e;
}

static inline void *deque_pop_back(struct deque *self)
{
    if (deque_empty(self)) {
        return NULL;
    }

    void *e = self->entry_[self->back_++];
    self->back_ = self->back_ == self->nr_entries_ ? 0 : self->back_;
    self->size_--;
    return e;
}

static inline void deque_swap(struct deque *self, struct deque *other)
{
    struct deque tmp = *self;

    *self = *other;
    *other = tmp;
}

static inline int __deque_alloc_entry(struct deque *self)
{
    self->entry_ = (void **)calloc(self->nr_entries_, sizeof(*self->entry_));
    return self->entry_ == NULL ? -ENOMEM : 0;
}

static inline void __deque_free_entry(struct deque *self)
{
    free(self->entry_);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_DEQUE_H__ */
