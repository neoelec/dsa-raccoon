/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/ardq.h>

int ardq_push_front(struct ardq *deque, void *e)
{
    if (ardq_is_full(deque))
        return -ERANGE;

    deque->count++;
    if (++deque->front == deque->nr_entries)
        deque->front = 0;

    deque->pool[deque->front] = e;

    return 0;
}

int ardq_push_rear(struct ardq *deque, void *e)
{
    if (ardq_is_full(deque))
        return -ERANGE;

    deque->count++;
    deque->rear = deque->rear == 0 ? deque->nr_entries - 1 : deque->rear - 1;

    deque->pool[deque->rear] = e;

    return 0;
}

int ardq_pop_front(struct ardq *deque, void **_e)
{
    if (ardq_is_empty(deque))
        return -ENODATA;

    if (_e != NULL)
        *_e = deque->pool[deque->front];

    deque->count--;
    deque->front = deque->front == 0 ? deque->nr_entries - 1 : deque->front - 1;

    return 0;
}

int ardq_pop_rear(struct ardq *deque, void **_e)
{
    if (ardq_is_empty(deque))
        return -ENODATA;

    if (_e != NULL)
        *_e = deque->pool[deque->rear];

    deque->count--;
    if (++deque->rear == deque->nr_entries)
        deque->rear = 0;

    return 0;
}

int ardq_front(const struct ardq *deque, void **_e)
{
    if (ardq_is_empty(deque))
        return -ENODATA;

    if (_e != NULL)
        *_e = deque->pool[deque->front];

    return 0;
}

int ardq_rear(const struct ardq *deque, void **_e)
{
    if (ardq_is_empty(deque))
        return -ENODATA;

    if (_e != NULL)
        *_e = deque->pool[deque->rear];

    return 0;
}

void ardq_forward(struct ardq *deque, do_each_t do_each, void *priv)
{
    ssize_t i, n, f, m, r;

    if (ardq_is_empty(deque))
        return;

    n = ardq_count(deque);
    f = (ssize_t)deque->front;
    m = (ssize_t)deque->nr_entries - 1;
    r = (ssize_t)deque->rear;

    for (i = f; i >= 0 && n > 0; i--, n--)
        do_each(deque->pool[i], priv);

    for (i = m; i >= r && n > 0; i--, n--)
        do_each(deque->pool[i], priv);
}

void ardq_backward(struct ardq *deque, do_each_t do_each, void *priv)
{
    size_t i, f, m, r;
    ssize_t n;

    if (ardq_is_empty(deque))
        return;

    n = ardq_count(deque);
    f = deque->front;
    m = deque->nr_entries - 1;
    r = deque->rear;

    for (i = r; i <= m && n > 0; i++, n--)
        do_each(deque->pool[i], priv);

    for (i = 0; i <= f && n > 0; i++, n--)
        do_each(deque->pool[i], priv);
}

void ardq_rotate(struct ardq *deque, ssize_t n)
{
    ssize_t count = ardq_count(deque);
    size_t i;

    if (n == 0 || n % count == 0)
        return;
    else if (n < 0)
        n = count + n % count;

    for (i = 0; i < (size_t)n; i++) {
        void *e;

        ardq_pop_front(deque, &e);
        ardq_push_rear(deque, e);
    }
}
