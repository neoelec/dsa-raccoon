/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Circular Queue */
#ifndef __CRQU_H__
#define __CRQU_H__

#include "common.h"

struct crqu {
    size_t nr_entries;
    size_t front;
    size_t rear;
    void *pool[];
};

#define CRQU_BYTES(nr_entries) \
    (sizeof(struct crqu) + (nr_entries) * (sizeof(void *)))

#define CRQU_MIN_NR_ENTRIES 8

static inline void crqu_initialize(struct crqu *queue, size_t nr_entries)
{
    assert(nr_entries >= CRQU_MIN_NR_ENTRIES);

    queue->nr_entries = nr_entries;
    queue->front = 0;
    queue->rear = 0;
}

static inline size_t crqu_count(const struct crqu *queue)
{
    if (queue->front <= queue->rear)
        return queue->rear - queue->front;
    else
        return queue->rear + queue->nr_entries - queue->front;
}

static inline bool crqu_is_empty(const struct crqu *queue)
{
    return queue->front == queue->rear;
}

static inline bool crqu_is_full(const struct crqu *queue)
{
    return (queue->rear + 1) % queue->nr_entries == queue->front;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int crqu_enqueue(struct crqu *queue, void *e);
extern int crqu_dequeue(struct crqu *queue, void **_e);
extern int crqu_front(const struct crqu *queue, void **_e);
extern int crqu_rear(const struct crqu *queue, void **_e);
extern void crqu_forward(struct crqu *queue, do_each_t do_each, void *priv);

#ifdef __cplusplus
}
#endif

static inline int crqu_enqueue_i(struct crqu *queue, intptr_t v)
{
    return crqu_enqueue(queue, (void *)v);
}

static inline int crqu_dequeue_i(struct crqu *queue, intptr_t *_v)
{
    return crqu_dequeue(queue, (void **)_v);
}

static inline int crqu_front_i(const struct crqu *queue, intptr_t *_v)
{
    return crqu_front(queue, (void **)_v);
}

static inline int crqu_rear_i(const struct crqu *queue, intptr_t *_v)
{
    return crqu_rear(queue, (void **)_v);
}

#endif /* __CRQU_H__ */
