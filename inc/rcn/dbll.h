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

static inline void dbll_node_initialize(struct dbll_node *x)
{
    x->prev = x;
    x->next = x;
}

static inline bool dbll_node_is_orphaned(const struct dbll_node *x)
{
    return x->next == x && x->prev == x;
}

static inline void dbll_node_link_next(struct dbll_node *x, struct dbll_node *y)
{
    struct dbll_node *z = x->next;

    /* x<->z => x<->y<->z */
    x->next = y;
    y->prev = x;
    z->prev = y;
    y->next = z;
}

static inline void dbll_node_link_prev(struct dbll_node *z, struct dbll_node *y)
{
    struct dbll_node *x = z->prev;

    /* x<->z => x<->y<->z */
    x->next = y;
    y->prev = x;
    z->prev = y;
    y->next = z;
}

static inline void dbll_node_unlink(struct dbll_node *y)
{
    struct dbll_node *x = y->prev;
    struct dbll_node *z = y->next;

    /* x<->y<->z => x<->z */
    x->next = z;
    z->prev = x;
}

struct dbll {
    size_t count;
    struct dbll_node pool;
};

static inline void dbll_initialize(struct dbll *list)
{
    dbll_node_initialize(&list->pool);
    list->count = 0;
}

static inline bool dbll_is_empty(const struct dbll *list)
{
    return list->count == 0;
}

static inline size_t dbll_count(const struct dbll *list)
{
    return list->count;
}

static inline void dbll_insert_head(struct dbll *list, struct dbll_node *x)
{
    dbll_node_link_next(&list->pool, x);
    list->count++;
}

static inline void dbll_insert_tail(struct dbll *list, struct dbll_node *x)
{
    dbll_node_link_prev(&list->pool, x);
    list->count++;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int dbll_node_search(const struct dbll_node *pool, dbll_compar_t compar,
                            const struct dbll_node *kx, struct dbll_node **_x);
extern void dbll_node_forward(struct dbll_node *pool, dbll_do_each_t do_each,
                              void *priv);
extern void dbll_node_backward(struct dbll_node *pool, dbll_do_each_t do_each,
                               void *priv);
extern void dbll_node_reverse(struct dbll_node *pool);

extern size_t dbll_count_debug(const struct dbll *list);
extern void dbll_link_next(struct dbll *list, struct dbll_node *x,
                           struct dbll_node *y);
extern void dbll_link_prev(struct dbll *list, struct dbll_node *z,
                           struct dbll_node *y);
extern void dbll_unlink(struct dbll *list, struct dbll_node *y);
extern int dbll_insert(struct dbll *list, size_t n, struct dbll_node *x);
extern int dbll_remvoe(struct dbll *list, size_t n, struct dbll_node **_x);
extern int dbll_remove_head(struct dbll *list, struct dbll_node **_x);
extern int dbll_remove_tail(struct dbll *list, struct dbll_node **_x);
extern int dbll_get(const struct dbll *list, size_t n, struct dbll_node **_x);
extern int dbll_get_head(const struct dbll *list, struct dbll_node **_x);
extern int dbll_get_tail(const struct dbll *list, struct dbll_node **_x);
extern int dbll_search(const struct dbll *list, dbll_compar_t compar,
                       const struct dbll_node *kx, struct dbll_node **_x);
extern void dbll_forward(struct dbll *list, dbll_do_each_t do_each, void *priv);
extern void dbll_backward(struct dbll *list, dbll_do_each_t do_each,
                          void *priv);
extern void dbll_reverse(struct dbll *list);

#ifdef __cplusplus
}
#endif

#endif /* __DBLL_H__ */
