/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Array Deque */
#ifndef __ARDQ_H__
#define __ARDQ_H__

#include "common.h"

struct ardq {
    size_t nr_entries;
    size_t count;
    size_t front;
    size_t rear;
    void *pool[];
};

#define ARDQ_BYTES(nr_entries) \
    (sizeof(struct ardq) + (nr_entries) * (sizeof(void *)))

#define ARDQ_MIN_NR_ENTRIES 8

static inline void ardq_initialize(struct ardq *deque, size_t nr_entries)
{
    assert(nr_entries >= ARDQ_MIN_NR_ENTRIES);

    deque->nr_entries = nr_entries;
    deque->count = 0;
    deque->front = deque->nr_entries - 1;
    deque->rear = 0;
}

static inline size_t ardq_count(const struct ardq *deque)
{
    return deque->count;
}

static inline bool ardq_is_empty(const struct ardq *deque)
{
    return deque->count == 0;
}

static inline bool ardq_is_full(const struct ardq *deque)
{
    return deque->count == deque->nr_entries;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int ardq_push_front(struct ardq *deque, void *e);
extern int ardq_push_rear(struct ardq *deque, void *e);
extern int ardq_pop_front(struct ardq *deque, void **_e);
extern int ardq_pop_rear(struct ardq *deque, void **_e);
extern int ardq_front(const struct ardq *deque, void **_e);
extern int ardq_rear(const struct ardq *deque, void **_e);
extern void ardq_forward(struct ardq *deque, do_each_t do_each, void *priv);
extern void ardq_backward(struct ardq *deque, do_each_t do_each, void *priv);
extern void ardq_rotate(struct ardq *deque, ssize_t n);

#ifdef __cplusplus
}
#endif

static inline int ardq_push_front_i(struct ardq *deque, intptr_t v)
{
    return ardq_push_front(deque, (void *)v);
}

static inline int ardq_push_rear_i(struct ardq *deque, intptr_t v)
{
    return ardq_push_rear(deque, (void *)v);
}

static inline int ardq_pop_front_i(struct ardq *deque, intptr_t *_v)
{
    return ardq_pop_front(deque, (void **)_v);
}

static inline int ardq_pop_rear_i(struct ardq *deque, intptr_t *_v)
{
    return ardq_pop_rear(deque, (void **)_v);
}

static inline int ardq_front_i(const struct ardq *deque, intptr_t *_v)
{
    return ardq_front(deque, (void **)_v);
}

static inline int ardq_rear_i(const struct ardq *deque, intptr_t *_v)
{
    return ardq_rear(deque, (void **)_v);
}

#endif /* __ARDQ_H__ */
