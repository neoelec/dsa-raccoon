/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Doubly Linked List */
#ifndef __DBLL_H__
#define __DBLL_H__

#include "common.h"

struct dbll_node;

typedef void (*dbll_do_each_t)(struct dbll_node *node, void *priv);
typedef int (*dbll_compar_t)(const struct dbll_node *key,
                             const struct dbll_node *in_list);

struct dbll_node {
    struct dbll_node *prev;
    struct dbll_node *next;
};

static inline void dbll_initialize(struct dbll_node *list)
{
    list->prev = list;
    list->next = list;
}

static inline bool dbll_is_empty(const struct dbll_node *list)
{
    return list->next == list && list->prev == list;
}

static inline void dbll_link_next(struct dbll_node *p, struct dbll_node *x)
{
    p->next->prev = x;
    x->next = p->next;
    p->next = x;
    x->prev = p;
}

static inline void dbll_link_previous(struct dbll_node *p, struct dbll_node *x)
{
    p->prev->next = x;
    x->prev = p->prev;
    p->prev = x;
    x->next = p;
}

static inline void dbll_unlink(struct dbll_node *z)
{
    z->prev->next = z->next;
    z->next->prev = z->prev;
    z->prev = NULL;
    z->next = NULL;
}

static inline void dbll_insert_head(struct dbll_node *list, struct dbll_node *x)
{
    dbll_link_next(list, x);
}

static inline void dbll_insert_tail(struct dbll_node *list, struct dbll_node *x)
{
    dbll_link_previous(list, x);
}

#ifdef __cplusplus
extern "C" {
#endif

extern size_t dbll_count(const struct dbll_node *list);
extern int dbll_insert(struct dbll_node *list, size_t n, struct dbll_node *x);
extern int dbll_remvoe(struct dbll_node *list, size_t n, struct dbll_node **_x);
extern int dbll_remove_head(struct dbll_node *list, struct dbll_node **_x);
extern int dbll_remove_tail(struct dbll_node *list, struct dbll_node **_x);
extern int dbll_get(const struct dbll_node *list, size_t n,
                    struct dbll_node **_x);
extern int dbll_get_head(const struct dbll_node *list, struct dbll_node **_x);
extern int dbll_get_tail(const struct dbll_node *list, struct dbll_node **_x);

extern int dbll_search(const struct dbll_node *list, dbll_compar_t compar,
                       const struct dbll_node *kx, struct dbll_node **_x);
extern void dbll_forward(struct dbll_node *list, dbll_do_each_t do_each,
                         void *priv);
extern void dbll_backward(struct dbll_node *list, dbll_do_each_t do_each,
                          void *priv);
extern void dbll_reverse(struct dbll_node *list);

#ifdef __cplusplus
}
#endif

#endif /* __DBLL_H__ */
