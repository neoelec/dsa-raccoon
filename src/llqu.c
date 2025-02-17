/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>

#include <rcn/llqu.h>

int llqu_enqueue(struct llqu *queue, struct dbll_node *x)
{
    queue->count++;
    dbll_insert_tail(&queue->list, x);

    return 0;
}

int llqu_insert_front(struct llqu *queue, struct dbll_node *x)
{
    queue->count++;
    dbll_insert_head(&queue->list, x);

    return 0;
}

int llqu_dequeue(struct llqu *queue, struct dbll_node **_x)
{
    if (llqu_is_empty(queue))
        return -ENODATA;

    queue->count--;
    dbll_remove_head(&queue->list, _x);

    return 0;
}

int llqu_remove_rear(struct llqu *queue, struct dbll_node **_x)
{
    if (llqu_is_empty(queue))
        return -ENODATA;

    queue->count--;
    dbll_remove_tail(&queue->list, _x);

    return 0;
}

int llqu_front(const struct llqu *queue, struct dbll_node **_x)
{
    if (llqu_is_empty(queue))
        return -ENODATA;

    dbll_get_head(&queue->list, _x);

    return 0;
}

int llqu_rear(const struct llqu *queue, struct dbll_node **_x)
{
    if (llqu_is_empty(queue))
        return -ENODATA;

    dbll_get_tail(&queue->list, _x);

    return 0;
}
