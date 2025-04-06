/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linked List Deque */
#ifndef __DEQUE_H__
#define __DEQUE_H__

#include "dbll.h"

static inline void lldq_initialize(struct dbll *deque)
{
    dbll_initialize(deque);
}

static inline size_t lldq_count(const struct dbll *deque)
{
    return dbll_count(deque);
}
static inline bool lldq_is_empty(const struct dbll *deque)
{
    return dbll_is_empty(deque);
}

static inline int lldq_insert(struct dbll *deque, ssize_t n,
                              struct dbll_node *x)
{
    ssize_t count = deque->count;

    if (n >= 0)
        return dbll_insert(deque, n % count, x);
    else
        return lldq_insert(deque, count + n % count, x);
}

static inline void lldq_push_front(struct dbll *deque, struct dbll_node *x)
{
    dbll_insert_head(deque, x);
}

static inline void lldq_push_rear(struct dbll *deque, struct dbll_node *x)
{
    dbll_insert_tail(deque, x);
}

static inline int lldq_remove(struct dbll *deque, ssize_t n,
                              struct dbll_node **_x)
{
    ssize_t count = deque->count;

    if (n >= 0)
        return dbll_remvoe(deque, n % count, _x);
    else
        return lldq_remove(deque, count + n % count, _x);
}

static inline int lldq_pop_front(struct dbll *deque, struct dbll_node **_x)
{
    return dbll_remove_head(deque, _x);
}

static inline int lldq_pop_rear(struct dbll *deque, struct dbll_node **_x)
{
    return dbll_remove_tail(deque, _x);
}

static inline int lldq_get(const struct dbll *deque, ssize_t n,
                           struct dbll_node **_x)
{
    ssize_t count = deque->count;

    if (n >= 0)
        return dbll_get(deque, n % count, _x);
    else
        return lldq_get(deque, count + n % count, _x);
}

static inline int lldq_front(const struct dbll *deque, struct dbll_node **_x)
{
    return dbll_get_head(deque, _x);
}

static inline int lldq_rear(const struct dbll *deque, struct dbll_node **_x)
{
    return dbll_get_tail(deque, _x);
}

static inline int lldq_search(const struct dbll *deque, dbll_compar_t compar,
                              const struct dbll_node *kx, struct dbll_node **_x)
{
    return dbll_search(deque, compar, kx, _x);
}

static inline void lldq_forward(struct dbll *deque, dbll_do_each_t do_each,
                                void *priv)
{
    dbll_forward(deque, do_each, priv);
}

static inline void lldq_backward(struct dbll *deque, dbll_do_each_t do_each,
                                 void *priv)
{
    dbll_backward(deque, do_each, priv);
}

static inline void lldq_reverse(struct dbll *deque)
{
    dbll_reverse(deque);
}

static inline void lldq_rotate(struct dbll *deque, ssize_t n)
{
    ssize_t count = deque->count;
    struct dbll_node *y;

    if (count == 0 || n % count == 0)
        return;

    lldq_get(deque, n, &y);
    dbll_node_unlink(&deque->pool);
    dbll_node_link_prev(y, &deque->pool);
}

#endif /* __DEQUE_H__ */
