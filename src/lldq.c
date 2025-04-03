/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/lldq.h>

static int __head(const struct lldq *deque, struct dbll_node **_x)
{
    *_x = deque->head;

    return lldq_is_empty(deque) ? -ENODATA : 0;
}

static int __tail(const struct lldq *deque, struct dbll_node **_x)
{
    *_x = deque->head ? deque->head->prev : NULL;

    return lldq_is_empty(deque) ? -ENODATA : 0;
}

static int __get(const struct lldq *deque, ssize_t n, struct dbll_node **_x,
                 struct dbll_node **_p)
{
    struct dbll_node *x;

    if (lldq_is_empty(deque))
        return -ENODATA;

    x = deque->head;
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

static void __remove(struct lldq *deque, struct dbll_node *x)
{
    if (x == deque->head)
        deque->head = x->next;

    dbll_node_unlink(x);

    if (deque->head == x)
        deque->head = NULL;

    deque->count--;
}

void lldq_insert(struct lldq *deque, ssize_t n, struct dbll_node *x)
{
    struct dbll_node *y, *p;
    int err;

    err = __get(deque, n, &y, &p);
    if (err != 0)
        dbll_node_initialize(x);
    else
        dbll_node_link_next(p, x);

    if (err != 0 || n == 0)
        deque->head = x;

    deque->count++;
}

void lldq_push_front(struct lldq *deque, struct dbll_node *x)
{
    struct dbll_node *h;
    int err;

    err = __head(deque, &h);
    if (err != 0)
        dbll_node_initialize(x);
    else
        dbll_node_link_prev(h, x);

    deque->head = x;
    deque->count++;
}

void lldq_push_rear(struct lldq *deque, struct dbll_node *x)
{
    struct dbll_node *t;
    int err;

    err = __tail(deque, &t);
    if (err != 0) {
        dbll_node_initialize(x);
        deque->head = x;
    } else
        dbll_node_link_next(t, x);

    deque->count++;
}

int lldq_remove(struct lldq *deque, ssize_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(deque, n, &x, &p);
    if (err != 0)
        return err;

    __remove(deque, x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int lldq_pop_front(struct lldq *deque, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(deque, &x);
    if (err != 0)
        return err;

    __remove(deque, x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int lldq_pop_rear(struct lldq *deque, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(deque, &x);
    if (err != 0)
        return err;

    __remove(deque, x);

    if (_x != NULL)
        *_x = x;

    return 0;
}

int lldq_get(const struct lldq *deque, ssize_t n, struct dbll_node **_x)
{
    struct dbll_node *x, *p;
    int err;

    err = __get(deque, n, &x, &p);

    if (_x != NULL)
        *_x = x;

    return err;
}

int lldq_front(const struct lldq *deque, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __head(deque, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int lldq_rear(const struct lldq *deque, struct dbll_node **_x)
{
    struct dbll_node *x;
    int err;

    err = __tail(deque, &x);

    if (_x != NULL)
        *_x = x;

    return err;
}

int lldq_search(const struct lldq *deque, dbll_compar_t compar,
                const struct dbll_node *kx, struct dbll_node **_x)
{
    struct dbll_node *x, *h;
    int err;

    err = __head(deque, &x);
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

void lldq_forward(struct lldq *deque, dbll_do_each_t do_each, void *priv)
{
    struct dbll_node *x, *h;
    int err;

    err = __head(deque, &x);
    if (err != 0)
        return;

    h = x;
    do {
        do_each(x, priv);
        x = x->next;
    } while (x != h);
}

void lldq_backward(struct lldq *deque, dbll_do_each_t do_each, void *priv)
{
    struct dbll_node *x, *t;
    int err;

    err = __tail(deque, &x);
    if (err != 0)
        return;

    t = x;
    do {
        do_each(x, priv);
        x = x->next;
    } while (x != t);
}

void lldq_reverse(struct lldq *deque)
{
    struct dbll_node __tmp, *tmp = &__tmp;

    if (lldq_is_empty(deque))
        return;

    dbll_node_link_prev(deque->head, tmp);
    dbll_node_reverse(tmp);

    deque->head = tmp->next;
    dbll_node_unlink(tmp);
}

void lldq_rotate(struct lldq *deque, ssize_t n)
{
    struct dbll_node *x, *p;

    if (lldq_is_empty(deque))
        return;

    __get(deque, n, &x, &p);
    deque->head = x;
}
