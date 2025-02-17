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

static inline void sgll_initialize(struct sgll_node *list)
{
    list->next = NULL;
}

static inline bool sgll_is_empty(const struct sgll_node *list)
{
    return list->next == NULL;
}

static inline void sgll_link_next(struct sgll_node *p, struct sgll_node *x)
{
    x->next = p->next;
    p->next = x;
}

static inline void sgll_insert_head(struct sgll_node *list, struct sgll_node *x)
{
    sgll_link_next(list, x);
}

#ifdef __cplusplus
extern "C" {
#endif

extern size_t sgll_count(const struct sgll_node *list);
extern int sgll_unlink_next(struct sgll_node *pos, struct sgll_node **_x);
extern int sgll_unlink(struct sgll_node *list, struct sgll_node *z);
extern int sgll_insert(struct sgll_node *list, size_t n, struct sgll_node *x);
extern void sgll_insert_tail(struct sgll_node *list, struct sgll_node *x);
extern int sgll_remvoe(struct sgll_node *list, size_t n, struct sgll_node **_x);
extern int sgll_remove_head(struct sgll_node *list, struct sgll_node **_x);
extern int sgll_remove_tail(struct sgll_node *list, struct sgll_node **_x);
extern int sgll_get(const struct sgll_node *list, size_t n,
                    struct sgll_node **_x);
extern int sgll_get_tail(const struct sgll_node *list, struct sgll_node **_x);
extern int sgll_get_head(const struct sgll_node *list, struct sgll_node **_x);
extern int sgll_search(const struct sgll_node *list, sgll_compar_t compar,
                       const struct sgll_node *kx, struct sgll_node **_x,
                       struct sgll_node **_p);
extern void sgll_forward(struct sgll_node *list, sgll_do_each_t do_each,
                         void *priv);
extern void sgll_reverse(struct sgll_node *list);

#ifdef __cplusplus
}
#endif

#endif /* __SGLL_H__ */
