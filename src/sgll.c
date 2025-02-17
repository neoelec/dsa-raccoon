/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/sgll.h>

static int __head(const struct sgll_node *list, struct sgll_node **_x)
{
    *_x = list->next;

    return sgll_is_empty(list) ? -ENODATA : 0;
}

static int __tail(const struct sgll_node *list, struct sgll_node **_x,
                  struct sgll_node **_p)
{
    struct sgll_node *x = list->next;
    struct sgll_node *p = (struct sgll_node *)list;

    if (x == NULL) {
        *_x = p;
        *_p = p;
        return -ERANGE;
    }

    while (x->next != NULL) {
        p = x;
        x = x->next;
    }

    *_x = x;
    *_p = p;

    return 0;
}

static void __unlink_next(struct sgll_node *p)
{
    struct sgll_node *x = p->next;

    p->next = x->next;
    x->next = NULL;
}

static int __get(const struct sgll_node *list, ssize_t n, struct sgll_node **_x,
                 struct sgll_node **_p)
{
    const struct sgll_node *x = list->next;
    const struct sgll_node *p = list;

    for (; n > 0 && x != NULL; --n) {
        p = x;
        x = x->next;
    }

    if (x == NULL)
        return -ERANGE;

    *_x = (struct sgll_node *)x;
    *_p = (struct sgll_node *)p;

    return 0;
}

size_t sgll_count(const struct sgll_node *list)
{
    const struct sgll_node *x;
    size_t count = 0;

    for (x = list->next; x != NULL; x = x->next)
        count++;

    return count;
}

int sgll_unlink_next(struct sgll_node *p, struct sgll_node **_x)
{
    if (p->next == NULL)
        return -ERANGE;

    if (_x != NULL)
        *_x = p->next;

    __unlink_next(p);

    return 0;
}

int sgll_unlink(struct sgll_node *list, struct sgll_node *z)
{
    struct sgll_node *p = list;

    while (p->next != z && p->next != NULL)
        p = p->next;

    if (p->next != z)
        return -ERANGE;

    __unlink_next(p);

    return 0;
}

int sgll_insert(struct sgll_node *list, size_t n, struct sgll_node *x)
{
    struct sgll_node *y, *p;
    int err;

    err = __get(list, n, &y, &p);
    if (err != 0)
        return -ERANGE;

    sgll_link_next(p, x);

    return 0;
}

void sgll_insert_tail(struct sgll_node *list, struct sgll_node *x)
{
    struct sgll_node *y, *p;

    __tail(list, &y, &p);
    sgll_link_next(y, x);
}

int sgll_remvoe(struct sgll_node *list, size_t n, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return -ERANGE;

    __unlink_next(p);

    if (_x == NULL)
        *_x = x;

    return 0;
}

int sgll_remove_head(struct sgll_node *list, struct sgll_node **_x)
{
    struct sgll_node *x;
    int err;

    err = __head(list, &x);

    list->next = x != NULL ? x->next : NULL;

    if (_x != NULL)
        *_x = x;

    return err;
}

int sgll_remove_tail(struct sgll_node *list, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __tail(list, &x, &p);

    if (p != NULL)
        p->next = NULL;

    if (_x != NULL)
        *_x = x;

    return err;
}

int sgll_get(const struct sgll_node *list, size_t n, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);

    if (_x != NULL)
        *_x = x;

    return err;
}

int sgll_get_head(const struct sgll_node *list, struct sgll_node **_x)
{
    struct sgll_node *x;
    int err;

    err = __head(list, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int sgll_get_tail(const struct sgll_node *list, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __tail(list, &x, &p);

    if (_x != NULL)
        *_x = x;

    return err;
}

int sgll_search(const struct sgll_node *list, sgll_compar_t compar,
                const struct sgll_node *kx, struct sgll_node **_x,
                struct sgll_node **_p)
{
    const struct sgll_node *x;
    const struct sgll_node *p = list;
    int ret;

    if (sgll_is_empty(list)) {
        x = list;
        ret = -ENOENT;
    } else {
        x = list->next;
        while (x != NULL && compar(kx, x) != 0) {
            p = x;
            x = x->next;
        }

        ret = x != NULL ? 0 : -ENOENT;
    }

    if (_x != NULL)
        *_x = (struct sgll_node *)x;

    if (_p != NULL)
        *_p = (struct sgll_node *)p;

    return ret;
}

void sgll_forward(struct sgll_node *list, sgll_do_each_t do_each, void *priv)
{
    struct sgll_node *x;

    for (x = list->next; x != NULL; x = x->next)
        do_each(x, priv);
}

void sgll_reverse(struct sgll_node *list)
{
    struct sgll_node *x = list->next;

    list->next = NULL;

    while (x != NULL) {
        struct sgll_node *y = x->next;

        x->next = list->next;
        list->next = x;
        x = y;
    }
}
