/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linked List Queue */
#ifndef __LLQU_H__
#define __LLQU_H__

#include "dbll.h"

struct llqu {
    size_t count;
    struct dbll_node list;
};

static inline void llqu_initialize(struct llqu *queue)
{
    queue->count = 0;
    dbll_initialize(&queue->list);
}

static inline size_t llqu_count(const struct llqu *queue)
{
    return queue->count;
}

static inline bool llqu_is_empty(const struct llqu *queue)
{
    return queue->count == 0;
}

static inline void llqu_forward(struct llqu *queue, dbll_do_each_t do_each,
                                void *priv)
{
    dbll_forward(&queue->list, do_each, priv);
}

static inline void llqu_backward(struct llqu *queue, dbll_do_each_t do_each,
                                 void *priv)
{
    dbll_backward(&queue->list, do_each, priv);
}

#ifdef __cplusplus
extern "C" {
#endif

extern int llqu_enqueue(struct llqu *queue, struct dbll_node *x);
extern int llqu_insert_front(struct llqu *queue, struct dbll_node *x);
extern int llqu_dequeue(struct llqu *queue, struct dbll_node **_x);
extern int llqu_remove_rear(struct llqu *queue, struct dbll_node **_x);
extern int llqu_front(const struct llqu *queue, struct dbll_node **_x);
extern int llqu_rear(const struct llqu *queue, struct dbll_node **_x);

#ifdef __cplusplus
}
#endif

static inline int llqu_insert_rear(struct llqu *queue, struct dbll_node *x)
{
    return llqu_enqueue(queue, x);
}

static inline int llqu_remove_front(struct llqu *queue, struct dbll_node **_x)
{
    return llqu_dequeue(queue, _x);
}

#endif /* __LLQU_H__ */
