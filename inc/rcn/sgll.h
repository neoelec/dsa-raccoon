/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Singly Linked List */
#ifndef __SGLL_H__
#define __SGLL_H__

#include "common.h"

struct sgll_node;

typedef void (*sgll_do_each_t)(struct sgll_node *node, void *priv);
typedef int (*sgll_compar_t)(const struct sgll_node *key,
                             const struct sgll_node *in_list);
struct sgll_node {
    struct sgll_node *next;
};

static inline void sgll_node_initialize(struct sgll_node *list)
{
    list->next = NULL;
}

static inline bool sgll_node_is_orphaned(const struct sgll_node *list)
{
    return list->next == NULL;
}

static inline void sgll_node_link_next(struct sgll_node *p, struct sgll_node *x)
{
    x->next = p->next;
    p->next = x;
}

static inline void sgll_node_unlink_next(struct sgll_node *p)
{
    struct sgll_node *x = p->next;

    if (x != NULL) {
        p->next = x->next;
        x->next = NULL;
    }
}

struct sgll {
    size_t count;
    struct sgll_node head;
};

static inline void sgll_initialize(struct sgll *list)
{
    sgll_node_initialize(&list->head);
    list->count = 0;
}

static inline bool sgll_is_empty(const struct sgll *list)
{
    return list->count == 0;
}

static inline size_t sgll_count(const struct sgll *list)
{
    return list->count;
}

static inline void sgll_insert_head(struct sgll *list, struct sgll_node *x)
{
    sgll_node_link_next(&list->head, x);
    list->count++;
}

#ifdef __cplusplus
extern "C" {
#endif

extern int sgll_node_search(const struct sgll_node *head, sgll_compar_t compar,
                            const struct sgll_node *kx, struct sgll_node **_x,
                            struct sgll_node **_p);
extern void sgll_node_forward(struct sgll_node *head, sgll_do_each_t do_each,
                              void *priv);
extern void sgll_node_reverse(struct sgll_node *head);

extern size_t sgll_count_debug(const struct sgll *list);
extern void sgll_link_next(struct sgll *list, struct sgll_node *x,
                           struct sgll_node *y);
extern void sgll_unlink_next(struct sgll *list, struct sgll_node *y);
extern int sgll_insert(struct sgll *list, size_t n, struct sgll_node *x);
extern int sgll_insert_tail(struct sgll *list, struct sgll_node *x);
extern int sgll_remvoe(struct sgll *list, size_t n, struct sgll_node **_x);
extern int sgll_remove_head(struct sgll *list, struct sgll_node **_x);
extern int sgll_remove_tail(struct sgll *list, struct sgll_node **_x);
extern int sgll_get(const struct sgll *list, size_t n, struct sgll_node **_x);
extern int sgll_get_head(const struct sgll *list, struct sgll_node **_x);
extern int sgll_get_tail(const struct sgll *list, struct sgll_node **_x);
extern int sgll_search(const struct sgll *list, sgll_compar_t compar,
                       const struct sgll_node *kx, struct sgll_node **_x,
                       struct sgll_node **_p);
extern void sgll_forward(struct sgll *list, sgll_do_each_t do_each, void *priv);
extern void sgll_reverse(struct sgll *list);

#ifdef __cplusplus
}
#endif

#endif /* __SGLL_H__ */
