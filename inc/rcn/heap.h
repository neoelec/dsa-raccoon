/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Heap */
#ifndef __HEAP_H__
#define __HEAP_H__

#include "common.h"

struct heap {
    compar_t compar;
    ssize_t order;
    ssize_t count;
    void **pool;
};

static inline bool heap_is_empty(const struct heap *heap)
{
    return !heap->count;
}

#ifdef __cplusplus
extern "C" {
#endif

extern void heap_initialize(struct heap *heap, compar_t compar);
extern int heap_insert(struct heap *heap, void *e);
extern int heap_remove(struct heap *heap, void **_e);
extern void heap_forward(struct heap *heap, do_each_t do_each, void *priv);

#ifdef __cplusplus
}
#endif

static inline int heap_insert_i(struct heap *heap, intptr_t v)
{
    return heap_insert(heap, (void *)v);
}

static inline int heap_remove_i(struct heap *heap, intptr_t *_v)
{
    return heap_remove(heap, (void **)_v);
}

#endif /* __HEAP_H__ */
