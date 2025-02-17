/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <rcn/llst.h>

int llst_push(struct llst *stack, struct sgll_node *x)
{
    stack->count++;
    sgll_link_next(&stack->list, x);

    return 0;
}

int llst_pop(struct llst *stack, struct sgll_node **_x)
{
    if (llst_is_empty(stack))
        return -ENODATA;

    stack->count--;

    return sgll_remove_head(&stack->list, _x);
}

int llst_top(const struct llst *stack, struct sgll_node **_x)
{
    if (llst_is_empty(stack))
        return -ENODATA;

    return sgll_get_head(&stack->list, _x);
}
