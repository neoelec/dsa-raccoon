/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Queue (Array) */
#ifndef __RCN_C_QUEUE_H__
#define __RCN_C_QUEUE_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct queue {
    size_t nr_entries_;
    void **entry_;
    size_t front_;
    size_t back_;
    size_t size_;
};

static inline void queue_clear(struct queue *self)
{
    self->front_ = self->nr_entries_ - 1;
    self->back_ = 0;
    self->size_ = 0;
}

static inline void queue_init(struct queue *self, size_t nr_entries,
                              void **entry)
{
    self->nr_entries_ = nr_entries;
    self->entry_ = entry;
    queue_clear(self);
}

static inline size_t queue_size(const struct queue *self)
{
    return self->size_;
}

static inline size_t queue_max_size(const struct queue *self)
{
    return self->nr_entries_;
}

static inline bool queue_empty(const struct queue *self)
{
    return queue_size(self) == 0;
}

static inline bool queue_full(const struct queue *self)
{
    return queue_size(self) == self->nr_entries_;
}

static inline void *queue_front(const struct queue *self)
{
    return queue_empty(self) ? NULL : self->entry_[self->front_];
}

static inline void *queue_back(const struct queue *self)
{
    return queue_empty(self) ? NULL : self->entry_[self->back_];
}

static inline void queue_push(struct queue *self, void *e)
{
    if (queue_full(self)) {
        return;
    }

    self->back_ = self->back_ == 0 ? self->nr_entries_ - 1 : self->back_ - 1;
    self->entry_[self->back_] = e;
    self->size_++;
}

static inline void *queue_pop(struct queue *self)
{
    if (queue_empty(self)) {
        return NULL;
    }

    void *e = self->entry_[self->front_];
    self->front_ = self->front_ == 0 ? self->nr_entries_ - 1 : self->front_ - 1;
    self->size_--;
    return e;
}

static inline void queue_swap(struct queue *self, struct queue *other)
{
    struct queue tmp;

    memcpy(&tmp, self, sizeof(tmp));
    memcpy(self, other, sizeof(tmp));
    memcpy(other, &tmp, sizeof(tmp));
}

static inline int __queue_alloc_entry(struct queue *self)
{
    self->entry_ = (void **)calloc(self->nr_entries_, sizeof(*self->entry_));
    return self->entry_ == NULL ? -ENOMEM : 0;
}

static inline void __queue_free_entry(struct queue *self)
{
    free(self->entry_);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_QUEUE_H__ */
