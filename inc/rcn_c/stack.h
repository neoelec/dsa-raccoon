/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Stack (Array) */
#ifndef __RCN_C_STACK_H__
#define __RCN_C_STACK_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
namespace rcn_c
{
#endif

struct stack {
    size_t nr_entries_;
    void **entry_;
    size_t top_;
};

static inline void stack_clear(struct stack *self)
{
    self->top_ = -1;
}

static inline void stack_init(struct stack *self, size_t nr_entries,
                              void **entry)
{
    self->nr_entries_ = nr_entries;
    self->entry_ = entry;
    stack_clear(self);
}

static inline size_t stack_size(const struct stack *self)
{
    return self->top_ + 1;
}

static inline size_t stack_max_size(const struct stack *self)
{
    return self->nr_entries_;
}

static inline bool stack_empty(const struct stack *self)
{
    return stack_size(self) == 0;
}

static inline bool stack_full(const struct stack *self)
{
    return stack_size(self) == self->nr_entries_;
}

static inline void *stack_top(const struct stack *self)
{
    if (stack_empty(self)) {
        return NULL;
    }

    return self->entry_[self->top_];
}

static inline int stack_push(struct stack *self, void *e)
{
    if (stack_size(self) == self->nr_entries_) {
        return -ENOSPC;
    }

    self->entry_[++self->top_] = e;

    return 0;
}

static inline void *stack_pop(struct stack *self)
{
    if (stack_empty(self)) {
        return NULL;
    }

    return self->entry_[self->top_--];
}

static inline void stack_swap(struct stack *self, struct stack *other)
{
    struct stack tmp = *self;

    *self = *other;
    *other = tmp;
}

static inline int __stack_alloc_entry(struct stack *self)
{
    self->entry_ = (void **)calloc(self->nr_entries_, sizeof(*self->entry_));
    return self->entry_ == NULL ? -ENOMEM : 0;
}

static inline void __stack_free_entry(struct stack *self)
{
    free(self->entry_);
}

#ifdef __cplusplus
}
#endif /* namespace rcn_c */

#endif /* __RCN_C_STACK_H__ */
