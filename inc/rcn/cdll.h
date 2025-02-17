/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Cicular Doubly Linked List */
#ifndef __CDLL_H__
#define __CDLL_H__

#include "dbll.h"

struct cdll {
    struct dbll_node *head;
};

static inline void cdll_initialize(struct cdll *list)
{
    list->head = NULL;
}

static inline bool cdll_is_empty(const struct cdll *list)
{
    return list->head == NULL;
}

#ifdef __cplusplus
extern "C" {
#endif
extern size_t cdll_count(const struct cdll *list);
extern void cdll_insert(struct cdll *list, ssize_t n, struct dbll_node *x);
extern void cdll_insert_head(struct cdll *list, struct dbll_node *x);
extern void cdll_insert_tail(struct cdll *list, struct dbll_node *x);
extern int cdll_remove(struct cdll *list, ssize_t n, struct dbll_node **_x);
extern int cdll_remove_head(struct cdll *list, struct dbll_node **_x);
extern int cdll_remove_tail(struct cdll *list, struct dbll_node **_x);
extern int cdll_get(const struct cdll *list, ssize_t n, struct dbll_node **_x);
extern int cdll_get_head(const struct cdll *list, struct dbll_node **_x);
extern int cdll_get_tail(const struct cdll *list, struct dbll_node **_x);
extern int cdll_search(const struct cdll *list, dbll_compar_t compar,
                       const struct dbll_node *kx, struct dbll_node **_x);
extern void cdll_forward(struct cdll *list, dbll_do_each_t do_each, void *priv);
extern void cdll_backward(struct cdll *list, dbll_do_each_t do_each,
                          void *priv);
extern void cdll_reverse(struct cdll *list);

#ifdef __cplusplus
}
#endif

#endif /* __CDLL_H__ */
