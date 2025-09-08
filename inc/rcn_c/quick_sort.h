/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Quick Sort */
#ifndef __RCN_C_QUICK_SORT_H__
#define __RCN_C_QUICK_SORT_H__

#include <stdlib.h>
#include <sys/types.h>

#include "swap.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

static ssize_t __partition(void *base, ssize_t left, ssize_t right, size_t size,
                           int (*compar)(const void *a, const void *b))
{
#define __base(n) (&((char *)base)[(n) * size])

    ssize_t pivot = left;
    ssize_t i = left;
    ssize_t j = right;

    while (i < j) {
        while (compar(__base(i), __base(pivot)) <= 0 && i < right) {
            i++;
        }

        while (compar(__base(j), __base(pivot)) > 0) {
            j--;
        }

        if (i < j) {
            swap(__base(i), __base(j), size);
        }
    }

    swap(__base(j), __base(pivot), size);

#undef __base

    return j;
}

static void __quick_sort(void *base, ssize_t left, ssize_t right, size_t size,
                         int (*compar)(const void *a, const void *b))
{
    if (left < right) {
        ssize_t j = __partition(base, left, right, size, compar);
        __quick_sort(base, left, j - 1, size, compar);
        __quick_sort(base, j + 1, right, size, compar);
    }
}

static inline void quick_sort(void *base, size_t nmemb, size_t size,
                              int (*compar)(const void *a, const void *b))
{
    __quick_sort(base, 0, nmemb - 1, size, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_QUICK_SORT_H__ */
