/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <rcn/heap.h>

#include "swap.h"
#include "utility.h"

#define __nr_entries(order) ((1U << (order)) - 1U)
#define __sz_pool(heap) (sizeof(*(heap)->pool) * __nr_entries(heap->order))
#define __compar(heap, a, b) (heap)->compar((heap)->pool[a], (heap)->pool[b])
#define __swap(heap, a, b) \
    swap(&((heap)->pool[a]), &((heap)->pool[b]), sizeof(*heap->pool))

static ssize_t __order(size_t nr_entries)
{
    ssize_t order = 0;

    while (nr_entries > __nr_entries(order))
        order++;

    return order;
}

static ssize_t __parent(ssize_t idx)
{
    return (idx - 1) / 2;
}

static ssize_t __child(ssize_t idx)
{
    return (2 * idx) + 1;
}

static void __initialize(struct heap *heap)
{
    heap->order = -1;
    heap->count = 0;
    heap->pool = NULL;
}

static void __realloc(struct heap *heap)
{
    heap->pool = realloc(heap->pool, __sz_pool(heap));
    if (heap->pool == NULL) {
        fprintf(stderr, "%s():%u: Failed to allocate memory for pool\n",
                __func__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

static void __expand(struct heap *heap)
{
    if (heap->order >= __order(heap->count))
        return;

    heap->order++;
    __realloc(heap);
}

static void __shrink(struct heap *heap)
{
    if (heap->order <= __order(heap->count))
        return;

    if (heap->count == 0) {
        __initialize(heap);
        free(heap->pool);
        return;
    }

    heap->order--;
    __realloc(heap);
}

void heap_initialize(struct heap *heap, compar_t compar)
{
    __initialize(heap);
    heap->compar = compar;
}

int heap_insert(struct heap *heap, void *e)
{
    swap_t swap;
    ssize_t pos = heap->count;
    ssize_t parent;
    int diff;

    parent = __parent(pos);

    heap->count++;
    __expand(heap);

    heap->pool[pos] = e;

    swap = __pick_swap(sizeof(*heap->pool));
    while (pos > 0) {
        diff = __compar(heap, pos, parent);
        if (diff >= 0)
            break;

        __swap(heap, pos, parent);
        pos = parent;
        parent = __parent(pos);
    }

    return 0;
}

int heap_remove(struct heap *heap, void **_e)
{
    swap_t swap;
    ssize_t parent = 0;
    ssize_t left;
    ssize_t right;

    if (heap->count == 0)
        return -ENOENT;

    if (_e != NULL)
        *_e = heap->pool[0];

    swap = __pick_swap(sizeof(*heap->pool));
    __swap(heap, 0, --heap->count);
    assert(heap->count >= 0);

    left = __child(0);
    right = left + 1;
    while (true) {
        ssize_t child = 0;

        if (left >= heap->count)
            break;

        if (right >= heap->count)
            child = left;
        else {
            if (__compar(heap, left, right) > 0)
                child = right;
            else
                child = left;
        }

        if (__compar(heap, child, parent) < 0) {
            __swap(heap, parent, child);
            parent = child;
        } else
            break;

        left = __child(parent);
        right = left + 1;
    }

    __shrink(heap);

    return 0;
}

void heap_forward(struct heap *heap, do_each_t do_each, void *priv)
{
    ssize_t i;

    for (i = 0; i < heap->count; i++)
        do_each(heap->pool[i], priv);
}
