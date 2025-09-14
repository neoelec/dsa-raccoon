/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Heap Sort */
#ifndef __RCN_C_HEAP_SORT_H__
#define __RCN_C_HEAP_SORT_H__

#include <sys/types.h>

#include "swap.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

static inline size_t __heap_left(size_t pos)
{
    return (2 * pos) + 1;
}

static inline void __down_heap(void *base, size_t nmemb, size_t pos,
                               size_t size,
                               int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    size_t child, left, right;
    left = __heap_left(pos);

    while (left < nmemb) {
        right = left + 1;

        if (right == nmemb) {
            child = left;
        } else {
            child = compar(__base(left), __base(right)) > 0 ? left : right;
        }

        if (compar(__base(pos), __base(child)) > 0) {
            break;
        }

        swap(__base(child), __base(pos), size);
        pos = child;
        left = __heap_left(pos);
    }

#undef __base
}

static inline void heap_sort(void *base, size_t nmemb, size_t size,
                             int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    for (ssize_t i = nmemb / 2 - 1; i >= 0; --i) {
        __down_heap(base, nmemb, i, size, compar);
    }

    for (ssize_t i = nmemb - 1; i >= 0; --i) {
        swap(__base(0), __base(i), size);
        __down_heap(base, i, 0, size, compar);
    }

#undef __base
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_HEAP_SORT_H__ */
