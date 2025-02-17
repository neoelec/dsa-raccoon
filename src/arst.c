/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/arst.h>

int arst_push(struct arst *stack, void *e)
{
    if (arst_is_full(stack))
        return -ERANGE;

    stack->pool[++stack->top] = e;

    return 0;
}

int arst_pop(struct arst *stack, void **_e)
{
    if (arst_is_empty(stack))
        return -ENODATA;

    if (_e != NULL)
        *_e = stack->pool[stack->top--];

    return 0;
}

int arst_top(const struct arst *stack, void **_e)
{
    if (arst_is_empty(stack))
        return -ENODATA;

    if (_e != NULL)
        *_e = stack->pool[stack->top];

    return 0;
}

void arst_forward(struct arst *stack, do_each_t do_each, void *priv)
{
    ssize_t i;

    for (i = 0; i <= stack->top; i++)
        do_each(stack->pool[i], priv);
}
