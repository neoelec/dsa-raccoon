/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/crqu.h>

int crqu_enqueue(struct crqu *queue, void *e)
{
    if (crqu_is_full(queue))
        return -ERANGE;

    queue->pool[queue->rear] = e;
    queue->rear = (queue->rear + 1) % queue->nr_entries;

    return 0;
}

int crqu_dequeue(struct crqu *queue, void **_e)
{
    if (crqu_is_empty(queue))
        return -ENODATA;

    if (_e != NULL)
        *_e = queue->pool[queue->front];

    queue->front = (queue->front + 1) % queue->nr_entries;

    return 0;
}

int crqu_front(const struct crqu *queue, void **_e)
{
    if (crqu_is_empty(queue))
        return -ENODATA;

    if (_e != NULL)
        *_e = queue->pool[queue->front];

    return 0;
}

int crqu_rear(const struct crqu *queue, void **_e)
{
    size_t i;

    if (crqu_is_empty(queue))
        return -ENODATA;

    i = (queue->rear + queue->nr_entries - 1) % queue->nr_entries;

    if (_e != NULL)
        *_e = queue->pool[i];

    return 0;
}

void crqu_forward(struct crqu *queue, do_each_t do_each, void *priv)
{
    size_t n;
    size_t i;

    n = crqu_count(queue);

    for (i = 0; i < n; i++) {
        size_t j = (queue->front + i) % queue->nr_entries;

        do_each(queue->pool[j], priv);
    }
}
