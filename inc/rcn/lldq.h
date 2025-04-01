/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linked List Deque */
#ifndef __LLDQ_H__
#define __LLDQ_H__

#include "dbll.h"

struct lldq {
    size_t count;
    struct dbll_node *head;
};

static inline void lldq_initialize(struct lldq *deque)
{
    deque->count = 0;
    deque->head = NULL;
}

static inline bool lldq_is_empty(const struct lldq *deque)
{
    return deque->head == NULL;
}

static inline size_t lldq_count(const struct lldq *deque)
{
    return deque->count;
}

#ifdef __cplusplus
extern "C" {
#endif
extern size_t lldq_count(const struct lldq *deque);
extern void lldq_insert(struct lldq *deque, ssize_t n, struct dbll_node *x);
extern void lldq_push_front(struct lldq *deque, struct dbll_node *x);
extern void lldq_push_rear(struct lldq *deque, struct dbll_node *x);
extern int lldq_remove(struct lldq *deque, ssize_t n, struct dbll_node **_x);
extern int lldq_pop_front(struct lldq *deque, struct dbll_node **_x);
extern int lldq_pop_rear(struct lldq *deque, struct dbll_node **_x);
extern int lldq_get(const struct lldq *deque, ssize_t n, struct dbll_node **_x);
extern int lldq_front(const struct lldq *deque, struct dbll_node **_x);
extern int lldq_rear(const struct lldq *deque, struct dbll_node **_x);
extern int lldq_search(const struct lldq *deque, dbll_compar_t compar,
                       const struct dbll_node *kx, struct dbll_node **_x);
extern void lldq_forward(struct lldq *deque, dbll_do_each_t do_each,
                         void *priv);
extern void lldq_backward(struct lldq *deque, dbll_do_each_t do_each,
                          void *priv);
extern void lldq_reverse(struct lldq *deque);
extern void lldq_rotate(struct lldq *deque, ssize_t n);

#ifdef __cplusplus
}
#endif

#endif /* __LLDQ_H__ */
