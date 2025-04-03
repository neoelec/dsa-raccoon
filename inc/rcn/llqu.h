/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linked List Queue */
#ifndef __LLQU_H__
#define __LLQU_H__

#include "dbll.h"

static inline void llqu_initialize(struct dbll *queue)
{
    dbll_initialize(queue);
}

static inline size_t llqu_count(const struct dbll *queue)
{
    return dbll_count(queue);
}

static inline bool llqu_is_empty(const struct dbll *queue)
{
    return dbll_is_empty(queue);
}

static inline void llqu_forward(struct dbll *queue, dbll_do_each_t do_each,
                                void *priv)
{
    dbll_forward(queue, do_each, priv);
}

static inline void llqu_enqueue(struct dbll *queue, struct dbll_node *x)
{
    dbll_insert_tail(queue, x);
}

static inline int llqu_dequeue(struct dbll *queue, struct dbll_node **_x)
{
    return dbll_remove_head(queue, _x);
}

static inline int llqu_front(const struct dbll *queue, struct dbll_node **_x)
{
    return dbll_get_head(queue, _x);
}

static inline int llqu_rear(const struct dbll *queue, struct dbll_node **_x)
{
    return dbll_get_tail(queue, _x);
}

#endif /* __LLQU_H__ */
