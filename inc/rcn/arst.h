/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Array Stack */
#ifndef __ARST_H__
#define __ARST_H__

#include "common.h"

struct arst {
    size_t nr_entries;
    ssize_t top;
    void *pool[];
};

#define ARST_BYTES(nr_entries) \
    (sizeof(struct arst) + (nr_entries) * (sizeof(void *)))

#define ARST_MIN_NR_ENTRIES 8

static inline void arst_initialize(struct arst *stack, size_t nr_entries)
{
    assert(nr_entries >= ARST_MIN_NR_ENTRIES);

    stack->nr_entries = nr_entries;
    stack->top = -1;
}

static inline size_t arst_count(const struct arst *stack)
{
    return stack->top + 1;
}

static inline bool arst_is_empty(const struct arst *stack)
{
    return stack->top == -1;
}

static inline bool arst_is_full(const struct arst *stack)
{
    return arst_count(stack) == stack->nr_entries;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int arst_push(struct arst *stack, void *e);
extern int arst_pop(struct arst *stack, void **_e);
extern int arst_top(const struct arst *stack, void **_e);
extern void arst_forward(struct arst *stack, do_each_t do_each, void *priv);

#ifdef __cplusplus
}
#endif

static inline int arst_push_i(struct arst *stack, intptr_t v)
{
    return arst_push(stack, (void *)v);
}

static inline int arst_pop_i(struct arst *stack, intptr_t *_v)
{
    return arst_pop(stack, (void **)_v);
}

static inline int arst_top_i(const struct arst *stack, intptr_t *_v)
{
    return arst_top(stack, (void **)_v);
}

#endif /* __ARST_H__ */
