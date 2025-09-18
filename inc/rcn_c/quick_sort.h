/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * Copyright (c) 2024-2025 YOUNGJIN JOO (neoelec@gmail.com)
 */

/* Quick Sort */
#ifndef __RCN_C_QUICK_SORT_H__
#define __RCN_C_QUICK_SORT_H__

#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include "swap.h"

#ifdef __cplusplus
namespace rcn_c
{
#endif

#define __base(n) (&((char *)base)[(n) * size])

static inline void
__median_of_three(void *base, ssize_t left, ssize_t right, size_t size,
                  int (*compar)(const void *a, const void *b))
{
#define __exchange(i, j)                        \
    do {                                        \
        if (compar(__base(i), __base(j)) > 0) { \
            swap(__base(i), __base(j), size);   \
        }                                       \
    } while (0)

    ssize_t samples[] = { left, (left + right) / 2, right };

    __exchange(samples[0], samples[1]);
    __exchange(samples[1], samples[2]);
    __exchange(samples[0], samples[1]);

    swap(__base(left), __base(samples[1]), size);

#undef __exchange
}

static inline void __random_pivot(void *base, ssize_t left, ssize_t right,
                                  size_t size,
                                  int (*compar)(const void *a, const void *b))
{
    ssize_t i = rand() % (int)(right - left + 1) + left;

    swap(__base(left), __base(i), size);
}

static inline ssize_t __partition(void *base, ssize_t left, ssize_t right,
                                  size_t size,
                                  int (*compar)(const void *a, const void *b))
{
    void *pivot = __base(left);
    ssize_t i = left;
    ssize_t j = right;

    while (i < j) {
        while (i < right && compar(__base(i), pivot) <= 0) {
            i++;
        }

        while (compar(__base(j), pivot) > 0) {
            j--;
        }

        if (i < j) {
            swap(__base(i), __base(j), size);
        }
    }

    swap(__base(j), pivot, size);

    return j;
}

#undef __base

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

static void __mquick_sort(void *base, ssize_t left, ssize_t right, size_t size,
                          int (*compar)(const void *a, const void *b))
{
    if (left < right) {
        ssize_t j;

        __median_of_three(base, left, right, size, compar);
        j = __partition(base, left, right, size, compar);
        __mquick_sort(base, left, j - 1, size, compar);
        __mquick_sort(base, j + 1, right, size, compar);
    }
}

static inline void mquick_sort(void *base, size_t nmemb, size_t size,
                               int (*compar)(const void *a, const void *b))
{
    __mquick_sort(base, 0, nmemb - 1, size, compar);
}

static void __rquick_sort(void *base, ssize_t left, ssize_t right, size_t size,
                          int (*compar)(const void *a, const void *b))
{
    if (left < right) {
        ssize_t j;

        __random_pivot(base, left, right, size, compar);
        j = __partition(base, left, right, size, compar);
        __rquick_sort(base, left, j - 1, size, compar);
        __rquick_sort(base, j + 1, right, size, compar);
    }
}

static inline void rquick_sort(void *base, size_t nmemb, size_t size,
                               int (*compar)(const void *a, const void *b))
{
    srand(time(NULL));
    __rquick_sort(base, 0, nmemb - 1, size, compar);
}

#ifdef __cplusplus
}
#endif

#endif /* __RCN_C_QUICK_SORT_H__ */
