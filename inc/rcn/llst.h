/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Linked List Stack */
#ifndef __LLST_H__
#define __LLST_H__

#include "sgll.h"

struct llst {
    size_t count;
    struct sgll_node list;
};

static inline void llst_initialize(struct llst *stack)
{
    stack->count = 0;
    sgll_initialize(&stack->list);
}

static inline size_t llst_count(const struct llst *stack)
{
    return stack->count;
}

static inline bool llst_is_empty(const struct llst *stack)
{
    return stack->count == 0;
}

static inline void llst_forward(struct llst *stack, sgll_do_each_t do_each,
                                void *priv)
{
    sgll_forward(&stack->list, do_each, priv);
}

#ifdef __cplusplus
extern "C" {
#endif

extern void llst_initialize(struct llst *stack);
extern int llst_push(struct llst *stack, struct sgll_node *x);
extern int llst_pop(struct llst *stack, struct sgll_node **_x);
extern int llst_top(const struct llst *stack, struct sgll_node **_x);
extern size_t llst_count(const struct llst *stack);
extern bool llst_is_empty(const struct llst *stack);
extern void llst_forward(struct llst *stack, sgll_do_each_t do_each,
                         void *priv);

#ifdef __cplusplus
}
#endif
#endif /* __LLST_H__ */
