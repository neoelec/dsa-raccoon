/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/arqu.h>

int arqu_enqueue(struct arqu *queue, void *e)
{
    if (arqu_is_full(queue))
        return -ERANGE;

    queue->rear = queue->rear == 0 ? queue->nr_entries - 1 : queue->rear - 1;
    queue->pool[queue->rear] = e;
    queue->count++;

    return 0;
}

int arqu_dequeue(struct arqu *queue, void **_e)
{
    if (arqu_is_empty(queue))
        return -ENODATA;

    if (_e != NULL)
        *_e = queue->pool[queue->front];

    queue->front = queue->front == 0 ? queue->nr_entries - 1 : queue->front - 1;
    queue->count--;

    return 0;
}

int arqu_front(const struct arqu *queue, void **_e)
{
    if (arqu_is_empty(queue))
        return -ENODATA;

    if (_e != NULL)
        *_e = queue->pool[queue->front];

    return 0;
}

int arqu_rear(const struct arqu *queue, void **_e)
{
    if (arqu_is_empty(queue))
        return -ENODATA;

    if (_e != NULL)
        *_e = queue->pool[queue->rear];

    return 0;
}

void arqu_forward(struct arqu *queue, do_each_t do_each, void *priv)
{
    ssize_t i;
    ssize_t n, r;

    if (arqu_is_empty(queue))
        return;

    n = arqu_count(queue);
    r = (ssize_t)queue->rear;

    for (i = queue->front; i >= 0 && n >= 0; i--, n--)
        do_each(queue->pool[i], priv);

    for (i = queue->nr_entries - 1; i >= r && n >= 0; i--, n--)
        do_each(queue->pool[i], priv);
}
