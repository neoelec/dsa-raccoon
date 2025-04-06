/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <assert.h>
#include <errno.h>

#include <rcn/dbll.h>

int dbll_node_search(const struct dbll_node *pool, dbll_compar_t compar,
                     const struct dbll_node *kx, struct dbll_node **_x)
{
    struct dbll_node *x;

    for (x = pool->next; x != pool; x = x->next) {
        if (compar(kx, x) == 0) {
            if (_x != NULL)
                *_x = x;

            return 0;
        }
    }

    return -ENOENT;
}

void dbll_node_forward(struct dbll_node *pool, dbll_do_each_t do_each,
                       void *priv)
{
    struct dbll_node *x;

    for (x = pool->next; x != pool; x = x->next)
        do_each(x, priv);
}

void dbll_node_backward(struct dbll_node *pool, dbll_do_each_t do_each,
                        void *priv)
{
    struct dbll_node *x;

    for (x = pool->prev; x != pool; x = x->prev)
        do_each(x, priv);
}

void dbll_node_reverse(struct dbll_node *pool)
{
    struct dbll_node *x = pool->next;

    do {
        struct dbll_node *z = x->next;

        dbll_node_unlink(z);
        dbll_node_link_next(pool, z);
    } while (x->next != pool);
}

static int __head(const struct dbll *list, struct dbll_node **_x)
{
    if (dbll_is_empty(list))
        return -ENODATA;

    *_x = list->pool.next;

    return 0;
}

static int __tail(const struct dbll *list, struct dbll_node **_x)
{
    const struct dbll_node *pool = &list->pool;

    if (dbll_is_empty(list))
        return -ENODATA;

    *_x = pool->prev;

    return 0;
}

static int __get(const struct dbll *list, ssize_t n, struct dbll_node **_x,
                 struct dbll_node **_p)
{
    const struct dbll_node *pool = &list->pool;
    struct dbll_node *x = pool->next;
    struct dbll_node *p = (struct dbll_node *)pool;

    if (dbll_is_empty(list))
        return -ENODATA;

    while (n-- > 0 && x != pool) {
        p = x;
        x = x->next;
    }

    if (x == pool)
        return -ERANGE;

    *_x = x;
    *_p = p;

    return 0;
}

size_t dbll_count_debug(const struct dbll *list)
{
    const struct dbll_node *pool = &list->pool;
    const struct dbll_node *x;
    size_t count = 0;

    for (x = pool->next; x != pool; x = x->next)
        count++;

    return count;
}

void dbll_link_next(struct dbll *list, struct dbll_node *x, struct dbll_node *y)
{
    dbll_node_link_next(x, y);
    list->count++;
}

void dbll_link_prev(struct dbll *list, struct dbll_node *x, struct dbll_node *y)
{
    dbll_node_link_prev(x, y);
    list->count++;
}

void dbll_unlink(struct dbll *list, struct dbll_node *y)
{
    dbll_node_unlink(y);
    list->count--;
}

int dbll_insert(struct dbll *list, size_t n, struct dbll_node *x)
{
    struct dbll_node *y, *p;
    int err;

    err = __get(list, n, &y, &p);
    if (err != 0)
        return err;

    dbll_node_link_next(p, x);
    list->count++;

    return 0;
}

int dbll_remvoe(struct dbll *list, size_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return err;

    dbll_node_unlink(x);
    list->count--;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_remove_head(struct dbll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return err;

    dbll_node_unlink(x);
    list->count--;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_remove_tail(struct dbll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(list, &x);
    if (err != 0)
        return err;

    dbll_node_unlink(x);
    list->count--;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_get(const struct dbll *list, size_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return err;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_get_head(const struct dbll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return err;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_get_tail(const struct dbll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(list, &x);
    if (err != 0)
        return err;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_search(const struct dbll *list, dbll_compar_t compar,
                const struct dbll_node *kx, struct dbll_node **_x)
{
    return dbll_node_search(&list->pool, compar, kx, _x);
}

void dbll_forward(struct dbll *list, dbll_do_each_t do_each, void *priv)
{
    dbll_node_forward(&list->pool, do_each, priv);
}

void dbll_backward(struct dbll *list, dbll_do_each_t do_each, void *priv)
{
    dbll_node_backward(&list->pool, do_each, priv);
}

void dbll_reverse(struct dbll *list)
{
    dbll_node_reverse(&list->pool);
}
