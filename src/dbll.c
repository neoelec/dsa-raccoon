/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/dbll.h>

static int __head(const struct dbll_node *list, struct dbll_node **_x)
{
    *_x = list->next;

    return dbll_is_empty(list) ? -ENODATA : 0;
}

static int __tail(const struct dbll_node *list, struct dbll_node **_x)
{
    *_x = list->prev;

    return dbll_is_empty(list) ? -ENODATA : 0;
}

static int __get(const struct dbll_node *list, ssize_t n, struct dbll_node **_x,
                 struct dbll_node **_p)
{
    struct dbll_node *x = list->next;
    struct dbll_node *p = (struct dbll_node *)list;

    if (dbll_is_empty(list))
        return -ENODATA;

    while (n-- > 0 && x != list) {
        p = x;
        x = x->next;
    }

    if (x == list)
        return -ERANGE;

    *_x = x;
    *_p = p;

    return 0;
}

size_t dbll_count(const struct dbll_node *list)
{
    const struct dbll_node *x;
    size_t count = 0;

    for (x = list->next; x != NULL; x = x->next)
        count++;

    return count;
}

int dbll_insert(struct dbll_node *list, size_t n, struct dbll_node *x)
{
    struct dbll_node *y, *p;
    int err;

    err = __get(list, n, &y, &p);
    if (err != 0)
        return err;

    dbll_link_next(p, x);

    return 0;
}

int dbll_remvoe(struct dbll_node *list, size_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return err;

    dbll_unlink(x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_remove_head(struct dbll_node *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return err;

    dbll_unlink(list->next);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_remove_tail(struct dbll_node *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(list, &x);
    if (err != 0)
        return err;

    dbll_unlink(list->next);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int dbll_get(const struct dbll_node *list, size_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);

    if (_x != NULL)
        *_x = x;

    return err;
}

int dbll_get_head(const struct dbll_node *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(list, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int dbll_get_tail(const struct dbll_node *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(list, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int dbll_search(const struct dbll_node *list, dbll_compar_t compar,
                const struct dbll_node *kx, struct dbll_node **_x)
{
    struct dbll_node *x;

    for (x = list->next; x != list; x = x->next) {
        if (compar(kx, x) == 0) {
            if (_x != NULL)
                *_x = x;

            return 0;
        }
    }

    return -ENOENT;
}

void dbll_forward(struct dbll_node *list, dbll_do_each_t do_each, void *priv)
{
    struct dbll_node *x;

    for (x = list->next; x != list; x = x->next)
        do_each(x, priv);
}

void dbll_backward(struct dbll_node *list, dbll_do_each_t do_each, void *priv)
{
    struct dbll_node *x;

    for (x = list->prev; x != list; x = x->prev)
        do_each(x, priv);
}

void dbll_reverse(struct dbll_node *list)
{
    struct dbll_node *x = list->next;

    do {
        struct dbll_node *y = x->next;

        dbll_unlink(y);
        dbll_link_next(list, y);
    } while (x->next != list);
}
