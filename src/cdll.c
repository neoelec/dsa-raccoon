/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/cdll.h>

static int __head(const struct cdll *list, struct dbll_node **_x)
{
    *_x = list->head;

    return cdll_is_empty(list) ? -ENODATA : 0;
}

static int __tail(const struct cdll *list, struct dbll_node **_x)
{
    *_x = list->head ? list->head->prev : NULL;

    return cdll_is_empty(list) ? -ENODATA : 0;
}

static int __get(const struct cdll *list, ssize_t n, struct dbll_node **_x,
                 struct dbll_node **_p)
{
    struct dbll_node *x;

    if (cdll_is_empty(list))
        return -ENODATA;

    x = list->head;
    if (n > 0) {
        while (--n >= 0)
            x = x->next;
    } else {
        while (++n <= 0)
            x = x->prev;
    }

    *_p = x->prev;
    *_x = x;

    return 0;
}

static void __remove(struct cdll *list, struct dbll_node *x)
{
    if (x == list->head)
        list->head = x->next;

    dbll_unlink(x);

    if (list->head == x)
        list->head = NULL;
}

size_t cdll_count(const struct cdll *list)
{
    struct dbll_node *x, *h;
    size_t count;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return err;

    count = 0;
    h = x;
    do {
        count++;
        x = x->next;
    } while (x != h);

    return count;
}

void cdll_insert(struct cdll *list, ssize_t n, struct dbll_node *x)
{
    struct dbll_node *y, *p;
    int err;

    err = __get(list, n, &y, &p);
    if (err != 0)
        dbll_initialize(x);
    else
        dbll_link_next(p, x);

    if (err != 0 || n == 0)
        list->head = x;
}

void cdll_insert_head(struct cdll *list, struct dbll_node *x)
{
    struct dbll_node *h;
    int err;

    err = __head(list, &h);
    if (err != 0)
        dbll_initialize(x);
    else
        dbll_link_previous(h, x);

    list->head = x;
}

void cdll_insert_tail(struct cdll *list, struct dbll_node *x)
{
    struct dbll_node *t;
    int err;

    err = __tail(list, &t);
    if (err != 0) {
        dbll_initialize(x);
        list->head = x;
    } else
        dbll_link_next(t, x);
}

int cdll_remove(struct cdll *list, ssize_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return err;

    __remove(list, x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int cdll_remove_head(struct cdll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return err;

    __remove(list, x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int cdll_remove_tail(struct cdll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(list, &x);
    if (err != 0)
        return err;

    __remove(list, x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int cdll_get(const struct cdll *list, ssize_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);

    if (_x != NULL)
        *_x = x;

    return err;
}

int cdll_get_head(const struct cdll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(list, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int cdll_get_tail(const struct cdll *list, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(list, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int cdll_search(const struct cdll *list, dbll_compar_t compar,
                const struct dbll_node *kx, struct dbll_node **_x)
{
    struct dbll_node *x, *h;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return err;

    h = x;
    do {
        if (compar(kx, x) == 0) {
            if (_x != NULL)
                *_x = x;

            return 0;
        }

        x = x->next;
    } while (x != h);

    return -ENOENT;
}

void cdll_forward(struct cdll *list, dbll_do_each_t do_each, void *priv)
{
    struct dbll_node *x, *h;
    int err;

    err = __head(list, &x);
    if (err != 0)
        return;

    h = x;
    do {
        do_each(x, priv);
        x = x->next;
    } while (x != h);
}

void cdll_backward(struct cdll *list, dbll_do_each_t do_each, void *priv)
{
    struct dbll_node *x, *t;
    int err;

    err = __tail(list, &x);
    if (err != 0)
        return;

    t = x;
    do {
        do_each(x, priv);
        x = x->next;
    } while (x != t);
}

void cdll_reverse(struct cdll *list)
{
    struct dbll_node __tmp, *tmp = &__tmp;

    if (cdll_is_empty(list))
        return;

    dbll_link_previous(list->head, tmp);
    dbll_reverse(tmp);

    list->head = tmp->next;
    dbll_unlink(tmp);
}

void cdll_rotate(struct cdll *list, ssize_t n)
{
    struct dbll_node *x, *p;

    if (cdll_is_empty(list))
        return;

    __get(list, n, &x, &p);
    list->head = x;
}
