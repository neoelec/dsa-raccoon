/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linked List Stack */
#ifndef __LLST_H__
#define __LLST_H__

#include "sgll.h"

static inline void llst_initialize(struct sgll *stack)
{
    sgll_initialize(stack);
}

static inline size_t llst_count(const struct sgll *stack)
{
    return sgll_count(stack);
}

static inline bool llst_is_empty(const struct sgll *stack)
{
    return sgll_is_empty(stack);
}

static inline void llst_forward(struct sgll *stack, sgll_do_each_t do_each,
                                void *priv)
{
    sgll_forward(stack, do_each, priv);
}

static inline void llst_push(struct sgll *stack, struct sgll_node *x)
{
    return sgll_insert_head(stack, x);
}

static inline int llst_pop(struct sgll *stack, struct sgll_node **_x)
{
    return sgll_remove_head(stack, _x);
}

static inline int llst_top(const struct sgll *stack, struct sgll_node **_x)
{
    return sgll_get_head(stack, _x);
}

#endif /* __LLST_H__ */
