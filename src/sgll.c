/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/sgll.h>

int sgll_node_search(const struct sgll_node *head, sgll_compar_t compar,
                     const struct sgll_node *kx, struct sgll_node **_x,
                     struct sgll_node **_p)
{
    const struct sgll_node *x;
    const struct sgll_node *p = head;
    int ret;

    if (sgll_node_is_orphaned(head)) {
        x = head;
        ret = -ENOENT;
    } else {
        x = head->next;
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

void sgll_node_forward(struct sgll_node *head, sgll_do_each_t do_each,
                       void *priv)
{
    struct sgll_node *x;

    for (x = head->next; x != NULL; x = x->next)
        do_each(x, priv);
}

void sgll_node_reverse(struct sgll_node *head)
{
    struct sgll_node *x = head->next;

    head->next = NULL;

    while (x != NULL) {
        struct sgll_node *y = x->next;

        x->next = head->next;
        head->next = x;
        x = y;
    }
}

static int __tail(const struct sgll *list, struct sgll_node **_x,
                  struct sgll_node **_p)
{
    const struct sgll_node *head = &list->head;
    struct sgll_node *x = head->next;
    struct sgll_node *p = (struct sgll_node *)head;

    if (x == NULL)
        x = p;

    while (x->next != NULL) {
        p = x;
        x = x->next;
    }

    *_x = x;
    *_p = p;

    return 0;
}

static int __get(const struct sgll *list, ssize_t n, struct sgll_node **_x,
                 struct sgll_node **_p)
{
    const struct sgll_node *head = &list->head;
    const struct sgll_node *x = head->next;
    const struct sgll_node *p = head;

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

size_t sgll_count_debug(const struct sgll *list)
{
    const struct sgll_node *head = &list->head;
    const struct sgll_node *x;
    size_t count = 0;

    for (x = head->next; x != NULL; x = x->next)
        count++;

    return count;
}

int sgll_insert(struct sgll *list, size_t n, struct sgll_node *x)
{
    struct sgll_node *y, *p;
    int err;

    err = __get(list, n, &y, &p);
    if (err != 0)
        return err;

    sgll_node_link_next(p, x);
    list->count++;

    return 0;
}

int sgll_insert_tail(struct sgll *list, struct sgll_node *y)
{
    struct sgll_node *x, *p;
    int err;

    err = __tail(list, &x, &p);
    if (err != 0)
        return err;

    sgll_node_link_next(x, y);
    list->count++;

    return 0;
}

int sgll_remvoe(struct sgll *list, size_t n, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return err;

    sgll_node_unlink_next(p);
    list->count--;

    if (_x == NULL)
        *_x = x;

    return 0;
}

int sgll_remove_head(struct sgll *list, struct sgll_node **_x)
{
    struct sgll_node *head = &list->head;

    if (sgll_is_empty(list))
        return -ENODATA;

    if (_x != NULL)
        *_x = head->next;

    sgll_node_unlink_next(head);
    list->count--;

    return 0;
}

int sgll_remove_tail(struct sgll *list, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __tail(list, &x, &p);
    if (err != 0)
        return err;

    sgll_node_unlink_next(p);
    list->count--;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int sgll_get(const struct sgll *list, size_t n, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __get(list, n, &x, &p);
    if (err != 0)
        return err;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int sgll_get_head(const struct sgll *list, struct sgll_node **_x)
{
    const struct sgll_node *head = &list->head;

    if (sgll_is_empty(list))
        return -ENODATA;

    if (_x != NULL)
        *_x = head->next;

    return 0;
}

int sgll_get_tail(const struct sgll *list, struct sgll_node **_x)
{
    struct sgll_node *x, *p;
    int err;

    err = __tail(list, &x, &p);
    if (err != 0)
        return err;

    if (_x != NULL)
        *_x = x;

    return 0;
}

int sgll_search(const struct sgll *list, sgll_compar_t compar,
                const struct sgll_node *kx, struct sgll_node **_x,
                struct sgll_node **_p)
{
    return sgll_node_search(&list->head, compar, kx, _x, _p);
}

void sgll_forward(struct sgll *list, sgll_do_each_t do_each, void *priv)
{
    return sgll_node_forward(&list->head, do_each, priv);
}

void sgll_reverse(struct sgll *list)
{
    sgll_node_reverse(&list->head);
}
