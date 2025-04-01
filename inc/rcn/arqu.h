/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Array Queue */
#ifndef __ARQU_H__
#define __ARQU_H__

#include "common.h"

struct arqu {
    size_t nr_entries;
    size_t count;
    size_t front;
    size_t rear;
    void *pool[];
};

#define ARQU_BYTES(nr_entries) \
    (sizeof(struct arqu) + (nr_entries) * (sizeof(void *)))

#define ARQU_MIN_NR_ENTRIES 8

static inline void arqu_initialize(struct arqu *queue, size_t nr_entries)
{
    assert(nr_entries >= ARQU_MIN_NR_ENTRIES);

    queue->nr_entries = nr_entries;
    queue->count = 0;
    queue->front = queue->nr_entries - 1;
    queue->rear = 0;
}

static inline size_t arqu_count(const struct arqu *queue)
{
    return queue->count;
}

static inline bool arqu_is_empty(const struct arqu *queue)
{
    return queue->count == 0;
}

static inline bool arqu_is_full(const struct arqu *queue)
{
    return queue->count == queue->nr_entries;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int arqu_enqueue(struct arqu *queue, void *e);
extern int arqu_dequeue(struct arqu *queue, void **_e);
extern int arqu_front(const struct arqu *queue, void **_e);
extern int arqu_rear(const struct arqu *queue, void **_e);
extern void arqu_forward(struct arqu *queue, do_each_t do_each, void *priv);

#ifdef __cplusplus
}
#endif

static inline int arqu_enqueue_i(struct arqu *queue, intptr_t v)
{
    return arqu_enqueue(queue, (void *)v);
}

static inline int arqu_dequeue_i(struct arqu *queue, intptr_t *_v)
{
    return arqu_dequeue(queue, (void **)_v);
}

static inline int arqu_front_i(const struct arqu *queue, intptr_t *_v)
{
    return arqu_front(queue, (void **)_v);
}

static inline int arqu_rear_i(const struct arqu *queue, intptr_t *_v)
{
    return arqu_rear(queue, (void **)_v);
}

#endif /* __ARQU_H__ */
